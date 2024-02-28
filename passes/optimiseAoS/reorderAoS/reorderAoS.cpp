#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LLVMContext.h"

#include  "llvm/Transforms/Utils/BasicBlockUtils.h"

#include "../../detectAoS/detectAoS.h"

// #include "../../detectAoS/detectAoS.cpp"

#include <string>

using namespace llvm;
using namespace std;

LLVMContext reorder_Context;
IRBuilder<> reorder_Builder(reorder_Context);

ArrayRef<Type*> elemArr; //original struct fields

vector<tuple<Type*,int,int,int,bool>> newSortedElems; //sorted struct fields

string modifiedStruct; //name of modified struct

bool performOpt = false;

namespace {

struct reorderAoS : public PassInfoMixin<reorderAoS> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {

        errs()<<"\n-------------------------- STRUCT FIELD REORDERING --------------------------\n\n";

        // use vector<StructType*> allStructs = M.getIdentifiedStructTypes()if vector<> confirmed is empty i.e only reorderAoS pass is called
        // otherwise use the confirmed vector to get all the used structs in AoS and store it in allStructs - if detectAoS and reorderAoS is called

        /// this is to be replaced with structs used in AoS 

        // bool noDynamic = false;

        // if(M.getFunction("malloc") == nullptr)
        // {
        //   noDynamic = true;
        // }

        vector<StructType*> allStructs;
        vector<pair<Value*,StructType*>> dynamicAoSList;

        if(detectAoSCalled == false)
        {
           errs()<<"Optimising all structs\n";
           allStructs = M.getIdentifiedStructTypes(); //get all struct types used in program
        }
        else
        {
          if(confirmed.size() == 0) //detectAoS pass has not been called
          {
            errs()<<"No AoS values found. Not applying struct field reordering.\n";
            errs()<<"\n----------------------- END OF STRUCT FIELD REORDERING -----------------------\n";
            return PreservedAnalyses::all();
          }
          else
          {
            //get structs used by all AoS
            for(int i = 0; i < confirmed.size(); i++)
            {
              StructType* structure = get<3>(confirmed.at(i));
              string type = get<2>(confirmed.at(i));

              if(structure == nullptr) //should not occur
              {
                errs()<<"WARNING: nullptr found!\n";
                continue;
              }

              if(type == "dynamic")
              {
                dynamicAoSList.push_back(make_pair(get<0>(confirmed.at(i)),structure));
              }

              bool exist = false;
              for(int j = 0; j < allStructs.size(); j++)
              {
                if(allStructs.at(j) == structure)
                {
                  exist = true;
                  break;
                }
              }
              if(exist == false)
              {
                allStructs.push_back(structure);
                errs()<<"Optimising "<<structure->getName()<<"\n";
              }
              else
              {
                errs()<<"Dupe found\n";
              }

            }
          }
        }

        allStructs.insert( allStructs.end(), coldStructs.begin(), coldStructs.end()); //adding cold pointer structs, created from structure splitting optimisation

        errs()<<"Optimising "<<allStructs.size()<<" struct(s), which are: \n";

        for(int i = 0; i < allStructs.size(); i++)
        {
          allStructs.at(i)->print(errs());
          errs()<<"\n";
          //get element type array
          elemArr = allStructs.at(i)->elements();


          ////////////// TEMPORARY /////////////////////////

          //don't reorder fields of structs that contain an array
          // bool containsArray = false;
          // for(auto e : elemArr)
          // {
          //   if(auto *AR = dyn_cast<ArrayType>(e))
          //   {
          //     containsArray = true;
          //     break;
          //   }
          // }

          // if(containsArray)
          //   continue;

          /////////////////////////////////////////////////

          vector<tuple<Type*,int,int,int,bool>> elems; //the type, original index, final index and size of each field will be kept track. the bool shows whether it is a bitfield.
        
          DataLayout* TD = new DataLayout(&M);

          int o = 0;
          // int oldStructSize = TD->getStructLayout(allStructs.at(i))->getSizeInBytes();
          int oldStructSize = 0;
          if(detectAoSCalled)
            oldStructSize = origStructSizes.find(allStructs.at(i))->second.second;
          else
            oldStructSize = TD->getTypeAllocSize(allStructs.at(i));
          int wordSize = 0;
          int currWord = 0; 

          /// get current struct fields and calculate the size of struct
          /// old index position of field is stored
          for(auto it = elemArr.begin(); it != elemArr.end(); it++)
          {
            Type* ty = const_cast<Type*>(*it);
            // int size = TD->getTypeAllocSize(ty); //includes allocation padding

            int size = TD->getTypeSizeInBits(ty);
            int pad = TD->getStructLayout(allStructs.at(i))->getElementOffsetInBits(o);
            // errs()<<" offset: "<<pad/8<<",  size: ";
            // errs()<<size/8<<"\n";

            Type* newTy;
            bool isBitfield = false;

            if(auto *AR = dyn_cast<ArrayType>(ty))
            {
                newTy = ty;
            }
            else if((size != 8) & (size != 32) & (size != 64) & (size < 64)) //for bitfields 
            {
              // Create an aggregate type (somehow) so offsets are created properly - check trello
              Type* I = IntegerType::getInt8Ty(reorder_Context);
              int num = size/8;
              newTy = ArrayType::get(I, num);
              isBitfield = true;

              // Type* newTy = Type::getIntNTy(reorder_Context,size); //create a new type equal in bit size to the original field
            }
            else if(ty->isPointerTy())
            {
              newTy = PointerType::get(reorder_Context,0);
            }
            else
            {
              newTy = Type::getIntNTy(reorder_Context,size); //create a new type equal in bit size to the original field
              newTy = ty;
            }

            //try and create a packed struct, and set alignment bits manually after sorting

            // Type* newTy = Type::getIntNTy(reorder_Context,size); //create a new type equal in bit size to the original field

            // elems.push_back(make_tuple(newTy,o,0,size/8)); //store each struct field, with its type, original index and size

            elems.push_back(make_tuple(newTy,o,0,size/8,isBitfield)); //store each struct field, with its type, original index and size

            o++;
          }

          //  for(int p = 0; p < elems.size(); p++)
          // {
          //   get<0>(elems.at(p))->print(errs());
          //   errs()<<"\n";
          //   errs()<<get<3>(elems.at(p));
          //   errs()<<"\n\n";
          // }

          // errs()<<"---------------------------\n";

          vector<tuple<Type*,int,int,int,bool>> sortedElems; //sorted struct fields

          /* sort fields by decreasing type size as well storing the new index position of the field */
          bool none = false;
          int f = 0;
          while(none == false)
          {
            none = true;
            int MAX = INT_MIN;
            int o; //old index
            vector<std::tuple<llvm::Type *, int, int, int, bool>>::iterator toDelete;
            for(auto it = elems.begin(); it != elems.end(); it++)
            {
              Type* ty = get<0>(*it);
              int size = get<3>(*it);
              if(size > MAX)
              {
                toDelete = it;
                MAX = size;
                none = false;
              }
            }

            if(elems.size() == 0) //exit if there are no longer any fields to sort
            {
              none = true;
            }
            else
            {
              sortedElems.push_back(make_tuple(get<0>(*toDelete),get<1>(*toDelete),f,get<3>(*toDelete),get<4>(*toDelete))); //store each struct field, with its type, original index, new index and size
              f++;
              elems.erase(toDelete);
            }

          }

          //  for(int p = 0; p < sortedElems.size(); p++)
          // {
          //   get<0>(sortedElems.at(p))->print(errs());
          //   errs()<<"\n";
          //   errs()<<get<3>(sortedElems.at(p));
          //   errs()<<"\n\n";
          // }

          int currIndex = 0;
          for(auto it1 = sortedElems.begin(); it1 != sortedElems.end(); it1)
          {
            int size = get<3>(*it1);
            int padding = 0;
            if(size > 8)
              padding = 8 - (size % 8);
            else
              padding = 8 - size;
            // errs()<<padding<<"\n";
            newSortedElems.push_back(make_tuple(get<0>(*it1),get<1>(*it1),currIndex,get<3>(*it1),get<4>(*it1)));
            sortedElems.erase(it1);
            currIndex++;
            if(padding == 0)
            {
              //taken outside

              // newSortedElems.push_back(*it);
              // sortedElems.erase(it);
            }
            else if(padding > 0)
            {
                for(auto it2 = sortedElems.begin(); it2 != sortedElems.end(); it2)
                {
                  if(padding == 0)
                    break;
                  size = get<3>(*it2);
                  if(size <= padding)
                  {
                    padding -= size;
                    newSortedElems.push_back(make_tuple(get<0>(*it2),get<1>(*it2),currIndex,get<3>(*it2),get<4>(*it2)));
                    sortedElems.erase(it2);
                    currIndex++;
                  }
                  else
                    it2++;
                }
            }
            // else
            //   it1++;
          }

          vector<Type*> newElems;

          /* Add padding */
          int currentWord = 8;
          for(int i = 0; i < newSortedElems.size(); i++)
          {
            // int size = get<3>(newSortedElems.at(i));
            // // errs()<<padding<<"\n";
            // if(currentWord >= size)
            // {
            //   newElems.push_back(get<0>(newSortedElems.at(i)));
            //   currentWord = currentWord - size;

            // }
            // else if((currentWord < size) & (currentWord != 0))
            // {
            //   Type* newTy = Type::getIntNTy(reorder_Context,currentWord*8); //create a new type equal in bit size to the original field
            //   newElems.push_back(newTy);
            //   currentWord = 8;
            // }

            // if(currentWord == 0)
            //   currentWord = 8;

            // if((i == newSortedElems.size() - 1) & (currentWord != 0))
            // {
            //   errs()<<"pad: "<<currentWord<<"\n";
            //   Type* newTy = Type::getIntNTy(reorder_Context,currentWord*8); //create a new type equal in bit size to the original field
            //   newElems.push_back(newTy);
            // }

            newElems.push_back(get<0>(newSortedElems.at(i)));
          }

          // vector<Type*> newElems;

          // int currentWord = 0;
          // for(int i = 0; i < newSortedElems.size(); i++)
          // {
          //   int size = get<3>(newSortedElems.at(i));
          //   // errs()<<size<<"\n";
          //   newElems.push_back(get<0>(newSortedElems.at(i)));
          // }

          // for(int i = 0; i < newElems.size(); i++)
          // {
          //   (newElems.at(i))->print(errs());
          //   errs()<<"\n";
          //   // newElems.push_back(get<0>(newSortedElems.at(i)));
          // }

          ArrayRef<Type*> elemArr = ArrayRef(newElems);

          // StructType* temp = StructType::get(reorder_Context,true);
          // temp->setBody(elemArr,true);

          StructType* temp = StructType::get(reorder_Context,false);
          temp->setBody(elemArr,false);

          // int p = 0;
          // for(auto it = elemArr.begin(); it != elemArr.end(); it++)
          // {
          //   Type* ty = const_cast<Type*>(*it);
          //   int size = TD->getTypeSizeInBits(ty);
          //   int pad = TD->getStructLayout(temp)->getElementOffsetInBits(p);
          //   // errs()<<" offset: "<<pad/8<<",  size: ";
          //   // errs()<<size/8<<"\n";
          //   p++;
          // }

          // int newStructSize = TD->getStructLayout(temp)->getSizeInBytes();
          int newStructSize = TD->getTypeAllocSize(temp);

          /* if new struct size is smaller, perform optimisation, otherwise skip */
          if(newStructSize < oldStructSize)
          {
            errs()<<"Performing optimisation. Saving "<<oldStructSize-newStructSize<<" bytes in struct size from "<<oldStructSize<<" bytes to "<<newStructSize<<" bytes.\n";
            performOpt = true;

            //update old and new size pair for this struct
            origStructSizes.find(allStructs.at(i))->second.first = origStructSizes.find(allStructs.at(i))->second.second;

            origStructSizes.find(allStructs.at(i))->second.second = newStructSize;
            
            // allStructs.at(i)->setBody(elemArr, true);
            allStructs.at(i)->setBody(elemArr, false);

            raw_string_ostream t_str(modifiedStruct);
            allStructs.at(i)->print(t_str);


            //if the AoS is dynamic, find its malloc() function call and update the allocation size
            for(int a = 0; a < dynamicAoSList.size(); a++)
            {
              // errs()<<"Dynamic AOS:"<<a<<"\n";
              if(allStructs.at(i) == dynamicAoSList.at(a).second)
              {
                // errs()<<"Updating malloc() operand\n";
                Value* aos = dynamicAoSList.at(a).first;
                //update malloc() call for this AoS
                for(auto u = aos->users().begin(); u != aos->users().end(); u++)
                {
                  Instruction* inst = cast<Instruction>(*u);
                  if(auto *SI = dyn_cast<StoreInst>(inst))
                  {
                    Instruction* pred = SI->getPrevNode();
                    if(auto *CI = dyn_cast<CallInst>(pred))
                    {
                      if(CI->getCalledFunction()->getName() == "malloc") //replace the malloc with a new one, allocating the reduced size
                      {
                        // mallocFound = true;
                        // Instruction* finalPred = CI->getPrevNode();
                        // if(auto *MI = dyn_cast<BinaryOperator>(finalPred))
                        // {
                        //   // Value* newSize = ConstantInt::get(M.getContext(),APInt(64,TD->getTypeAllocSize(currentStruct) + 8));
                        //   int newSize = origStructSizes.find(currentStruct)->second;
                        //   Value* newSizeOperand = ConstantInt::get(M.getContext(),APInt(64,newSize)); //get the correct size and insert it here
                        //   MI->setOperand(1,newSizeOperand);
                        // }
                        // //can be a constant

                        // //can be a variable

                        Instruction* next = CI->getNextNode();
                        
                        int newSize = origStructSizes.find(allStructs.at(i))->second.second;
                        Value* structSize = ConstantInt::get(M.getContext(),APInt(64,newSize));

                        IRBuilder<> TmpB(next);

                        int origSize = origStructSizes.find(allStructs.at(i))->second.first;
                        Value* origSizeValue = ConstantInt::get(M.getContext(),APInt(64,origSize));

                        Value* numElements = TmpB.CreateUDiv(CI->getArgOperand(0),origSizeValue,"");
                        Value* newAllocSize = TmpB.CreateMul(numElements,structSize,"",false,false);

                        CallInst* mallocInst = TmpB.CreateCall(M.getFunction("malloc"),newAllocSize,"",nullptr);
                        CI->replaceAllUsesWith(mallocInst);
                        CI->eraseFromParent();

                        //we can use this calculated array size to free the AoS later
                      }
                      else if(CI->getCalledFunction()->getName() == "calloc") //update second operand of calloc with new struct size
                      {
                        // mallocFound = true;
                        // Instruction* next = CI->getNextNode();
                        
                        int newSize = origStructSizes.find(allStructs.at(i))->second.second;
                        Value* structSize = ConstantInt::get(M.getContext(),APInt(64,newSize));

                        CI->setArgOperand(1,structSize);
                      }
                      // do one for realloc() and test it
                    }
                    // else if(CI->getCalledFunction()->getName() == "realloc") //update second operand of realloc
                    // {
                    //   // mallocFound = true;
                    //     IRBuilder<> TmpB(CI);
                        
                    //     int newSize = origStructSizes.find(allStructs.at(i))->second.second;
                    //     Value* structSize = ConstantInt::get(M.getContext(),APInt(64,newSize));

                    //     int origSize = origStructSizes.find(allStructs.at(i))->second.first;
                    //     Value* origSizeValue = ConstantInt::get(M.getContext(),APInt(64,origSize));

                    //     Value* numElements = TmpB.CreateUDiv(CI->getArgOperand(1),origSizeValue,"");
                    //     Value* newAllocSize = TmpB.CreateMul(numElements,structSize,"",false,false);

                    //     CI->setArgOperand(1,newAllocSize);
                    // }
                  }
                }
              }
              else
                continue;
            }

          }
          else
          {
            errs()<<"Not performing optimisation. ";
            if(newStructSize > oldStructSize)
              errs()<<"New size is larger than old struct by "<<newStructSize-oldStructSize<<" bytes from "<<oldStructSize<<" bytes to "<<newStructSize<<" bytes.\n";
            else
              errs()<<"New struct has same size as old struct: "<<newStructSize<<" bytes.\n";

            elems.clear();
            sortedElems.clear();
            newSortedElems.clear();
            newElems.clear();
            continue;
          }

          /* Now replace the last index of GEP instructions that use the optimised struct */

          /// changing indices of all GEP instructions that use the modified struct
          if(performOpt == true)
          {
            for (auto &F : M) 
            { 
              for (auto &B : F)  
              { 
                  bool foundAoS = false;
                  bool lookForStore = false;
                  GetElementPtrInst* bitfieldGEP;
                  Type* actualType;
                  for (auto &I : B) /// iterate through all instructions
                  {
                    if(lookForStore == true) //if a value is being stored in a bitfield of a struct, may need to change its type
                    {
                      if(auto *SI = dyn_cast<StoreInst>(&I)) //find a Store instruction for a bitfield
                      {
                        Value* operand = SI->getOperand(0); //get operand that is being stored in bitfield
                        
                        int origSize = TD->getTypeSizeInBits(operand->getType())/8; //get size of this operand to be stored

                        int newSize = TD->getTypeSizeInBits(actualType)/8; //get intended size that should be stored in bitfield


                        if(origSize != newSize) //change store operand only if allocated size is greater than actual size - to make sure the data fits in a cache line without overflowing - it doesn't matter if the size is less that actual, this is guaranteed to fit
                        {
                          TruncInst* newTrunc = new TruncInst(operand,Type::getIntNTy(reorder_Context,newSize*8),"",&I); 
                          SI->setOperand(0,newTrunc);
                        }

                        lookForStore = false;
                      }
                    }
                    if(auto *GEP = dyn_cast<GetElementPtrInst>(&I)) //find a GEP instruction
                    {
                      lookForStore = false; //if a GEP is found before a store, it means that nothing is being stored in that index, only accessed
                      Value* ret = GEP->getOperand(GEP->getNumIndices()); //getting last operand
                      string op_str; 
                      raw_string_ostream to_str(op_str);
                      ret->printAsOperand(to_str);

                      Type* structUsed = GEP->getSourceElementType(); //get the struct ptr that is being accessed
                      string ret_str; 
                      raw_string_ostream t_str(ret_str);
                      structUsed->print(t_str);

                      /* An access to an AoS elements involves two GEP instructions:
                        - First to access the struct pointer - source element type - type struct
                        - Second to access field of the struct pointer - source element type - same as the field type
                      */

                      //These two operations should exists within the same block, since the flag is set to false in a new block

                      if(ret_str == modifiedStruct) //if an access to the modified AoS is found, turn flag on to change its GEP operations
                        foundAoS = true;

                      /* Only access/change indicies of GEP instruction if:
                        - it is not equal to a % value - it should only be equal to a i32 value
                        - the result element type is not a struct - this should be identified in the preceding GEP (access to the struct ptr)
                        - if the modified struct is being accessed in the preceding GEP - indicated by this boolean flag
                      */ 

                      if((op_str.find('%') == string::npos) & (!(GEP->getResultElementType()->isStructTy())) & (foundAoS == true)) 
                      {
                        size_t space_pos = op_str.find(" ");    
                        string type;
                        string indexStr;

                        //string manipulation to get 'type' and 'name' of the indices
                        if (space_pos != std::string::npos) 
                        {
                          type = op_str.substr(0, space_pos);
                        }

                        indexStr = op_str.substr(space_pos + 1);

                        int index;

                        if(indexStr.find("%") == string::npos)
                          index = std::stoi(indexStr);

                        /// replace old indices with new index positions (if the field has changed position)
                        for(int i = 0; i < newSortedElems.size(); i++)
                        {
                          int currIndex = get<1>(newSortedElems.at(i));
                          int newIndex = get<2>(newSortedElems.at(i));
                          bool isBitfield = get<4>(newSortedElems.at(i));
                          // errs()<<currIndex<<" -> "<<newIndex<<"\n";

                          if(index == currIndex) //replace old index with new index
                          {
                            index = newIndex;
                            GEP->setResultElementType(get<0>(newSortedElems.at(i)));

                            if(isBitfield == true)
                            {
                              lookForStore = true;
                              actualType = get<0>(newSortedElems.at(i));
                              bitfieldGEP = GEP;
                              // errs()<<"Found bitfield\n";
                              // I.print(errs());
                              // errs()<<"\n";
                            }

                            break;
                          }
                        }

                        GEP->setOperand(GEP->getNumIndices(),ConstantInt::get(reorder_Context,APInt(32,index))); //new index for the GEP instruction is set
                        foundAoS = false;

                      }
                    }
                  }
              }
            }
          }

          /// reset values and arrays and continue to reorder next AoS struct
          newSortedElems.clear();
          performOpt = false;
          modifiedStruct = "";

          elems.clear();
          newSortedElems.clear();
          newElems.clear();
          
        }

        //Set to ::all() if IR is unchanged, otherwise ::none()
        errs()<<"\n----------------------- END OF STRUCT FIELD REORDERING -----------------------\n";
        return PreservedAnalyses::none();
    };
};

}

// //Creates plugin for pass - required
// extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
// llvmGetPassPluginInfo() {
//   return {
//     LLVM_PLUGIN_API_VERSION, "reorderAoS", "v0.1",
//     [](PassBuilder &PB) {
//       PB.registerPipelineParsingCallback(
//         [](StringRef Name, ModulePassManager &MPM, //For FunctionPass use FunctionPassManager &FPM
//         ArrayRef<PassBuilder::PipelineElement>) {
//           if(Name == "reorderAoS"){ //name of pass
//             MPM.addPass(reorderAoS());
//             return true;
//           }
//           return false;
//         }
//       );
//     }
//   };
// }