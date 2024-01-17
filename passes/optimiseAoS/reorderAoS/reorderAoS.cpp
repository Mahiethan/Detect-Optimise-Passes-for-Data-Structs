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

LLVMContext TheContext;
IRBuilder<> Builder(TheContext);

ArrayRef<Type*> elemArr; //original struct fields

vector<tuple<Type*,int,int,int,bool>> newSortedElems; //sorted struct fields

string modifiedStruct; //name of modified struct

bool performOpt = false;

namespace {

struct reorderAoS : public PassInfoMixin<reorderAoS> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {

        // use vector<StructType*> allStructs = M.getIdentifiedStructTypes()if vector<> confirmed is empty i.e only reorderAoS pass is called
        // otherwise use the confirmed vector to get all the used structs in AoS and store it in allStructs - if detectAoS and reorderAoS is called

        /// this is to be replaced with structs used in AoS 

        vector<StructType*> allStructs;

        if(confirmed.size() == 0) //detectAoS pass has not been called
        {
          errs()<<"Optimising all structs\n";
          allStructs = M.getIdentifiedStructTypes(); //get all struct types used in program
        }
        else
        {
          //get structs used by all AoS
          errs()<<"To be implemented\n";
        }

        for(int i = 0; i < allStructs.size(); i++)
        {
          //get element type array
          elemArr = allStructs.at(i)->elements();

          vector<tuple<Type*,int,int,int,bool>> elems; //the type, original index, final index and size of each field will be kept track. the bool shows whether it is a bitfield.
        
          DataLayout* TD = new DataLayout(&M);

          int o = 0;
          int oldStructSize = TD->getStructLayout(allStructs.at(i))->getSizeInBytes();
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

            if((size != 8) & (size != 32) & (size != 64))
            {
              // Create an aggregate type (somehow) so offsets are created properly - check trello
              Type* I = IntegerType::getInt8Ty(TheContext);
              int num = size/8;
              newTy = ArrayType::get(I, num);
              isBitfield = true;

              // Type* newTy = Type::getIntNTy(TheContext,size); //create a new type equal in bit size to the original field
            }
            else
            {
               newTy = Type::getIntNTy(TheContext,size); //create a new type equal in bit size to the original field
              newTy = ty;
            }

            //try and create a packed struct, and set alignment bits manually after sorting

            // Type* newTy = Type::getIntNTy(TheContext,size); //create a new type equal in bit size to the original field

            // elems.push_back(make_tuple(newTy,o,0,size/8)); //store each struct field, with its type, original index and size

            elems.push_back(make_tuple(newTy,o,0,size/8,isBitfield)); //store each struct field, with its type, original index and size

            o++;
          }

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

          int currIndex = 0;
          for(auto it1 = sortedElems.begin(); it1 != sortedElems.end(); it1)
          {
            int size = get<3>(*it1);
            int padding = 8 - size;
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
            else
              it1++;
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
            //   Type* newTy = Type::getIntNTy(TheContext,currentWord*8); //create a new type equal in bit size to the original field
            //   newElems.push_back(newTy);
            //   currentWord = 8;
            // }

            // if(currentWord == 0)
            //   currentWord = 8;

            // if((i == newSortedElems.size() - 1) & (currentWord != 0))
            // {
            //   errs()<<"pad: "<<currentWord<<"\n";
            //   Type* newTy = Type::getIntNTy(TheContext,currentWord*8); //create a new type equal in bit size to the original field
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

          ArrayRef<Type*> elemArr = ArrayRef(newElems);

          // StructType* temp = StructType::get(TheContext,true);
          // temp->setBody(elemArr,true);

          StructType* temp = StructType::get(TheContext,false);
          temp->setBody(elemArr,false);

          int p = 0;
          for(auto it = elemArr.begin(); it != elemArr.end(); it++)
          {
            Type* ty = const_cast<Type*>(*it);
            int size = TD->getTypeSizeInBits(ty);
            int pad = TD->getStructLayout(temp)->getElementOffsetInBits(p);
            // errs()<<" offset: "<<pad/8<<",  size: ";
            // errs()<<size/8<<"\n";
            p++;
          }

          int newStructSize = TD->getStructLayout(temp)->getSizeInBytes();

          /* if new struct size is smaller, perform optimisation, otherwise skip */
          if(newStructSize < oldStructSize)
          {
            errs()<<"Performing optimisation. Saving "<<oldStructSize-newStructSize<<" bytes in struct size from "<<oldStructSize<<" bytes to "<<newStructSize<<" bytes.\n";
            performOpt = true;
            
            // allStructs.at(i)->setBody(elemArr, true);
            allStructs.at(i)->setBody(elemArr, false);

            raw_string_ostream t_str(modifiedStruct);
            allStructs.at(i)->print(t_str);
          }
          else
          {
            errs()<<"Not performing optimisation. ";
            if(newStructSize > oldStructSize)
              errs()<<"New size is larger than old struct by "<<newStructSize-oldStructSize<<" bytes from "<<oldStructSize<<" bytes to "<<newStructSize<<" bytes.\n";
            else
              errs()<<"New struct has same size as old struct: "<<newStructSize<<" bytes.\n";
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
                          TruncInst* newTrunc = new TruncInst(operand,Type::getIntNTy(TheContext,newSize*8),"",&I); 
                          SI->setOperand(0,newTrunc);
                        }

                        lookForStore = false;
                      }
                    }
                    //also look out for load inst that has a GEP as its operand, then change this GEP indices
                    // if(auto *LI = dyn_cast<LoadInst>(&I))
                    // {
                    //   Value* operand = LI->getOperand(0);
                    //   operand->print(errs());
                    //   errs()<<"\n";


                    //   if(auto *GEP = dyn_cast<GEPOperator>(operand))
                    //   {
                    //     // if(GEP->getSourceElementType() == allStructs.at(i))
                    //     // {
                    //       errs()<<"yhh\n";
                    //       Value* ptrOperand = GEP->getPointerOperand();
                          
                    //       // GEP->setOperand(GEP->getNumIndices(),ConstantInt::get(TheContext,APInt(32,7)));
                    //       // GEP->print(errs());

                    //     // }
                    //     // GEP->getSourceElementType()->print(errs());
                    //     // errs()<<"\n";

                    //   }
                    // }
                    // else 
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

                        GEP->setOperand(GEP->getNumIndices(),ConstantInt::get(TheContext,APInt(32,index))); //new index for the GEP instruction is set
                        foundAoS = false;

                      }
                    }

                    // //set correct alignments when storing into a struct - no effect
                    // if(auto *SI = dyn_cast<StoreInst>(&I))
                    // {
                    //   //get second operand
                    //   Value* oper = SI->getOperand(1);
                    //   string op_str; 
                    //   raw_string_ostream to_str(op_str);
                    //   oper->printAsOperand(to_str);
                    //   errs()<<"\n";
                    //   if(op_str.find("ptr %e") != string::npos)
                    //   {
                    //     errs()<<"found %e\n";
                    //     SI->setAlignment(Align(2));
                    //   }
                      

                    // }
                  }
              }
            }
          }

          /// reset values and arrays and continue to reorder next AoS struct
          newSortedElems.clear();
          performOpt = false;
          modifiedStruct = "";
          
        }

        //Set to ::all() if IR is unchanged, otherwise ::none()
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