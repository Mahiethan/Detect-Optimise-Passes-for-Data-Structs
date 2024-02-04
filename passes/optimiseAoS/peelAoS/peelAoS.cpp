#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/Analysis/LoopInfo.h"

#include "../../detectAoS/detectAoS.h"

using namespace llvm;
using namespace std;

LLVMContext peel_Context;
IRBuilder<> peel_Builder(peel_Context);

// void runOnLoop(Loop* L) {
//   int Count = 0;
//   //following for loop does not work
//   // for (auto Iter = L->block_begin(), End = L->block_end(); Iter != End; ++Iter) {
//   //   // Count += Iter->size();
//   //   errs()<<"yes\n";
//   // }
//   // Do something with Count
//   // return false;
// }

//iterate through all structs
          //iterate through all globals
            //find globals that use the curr struct and add to list
          //if list > 0, apply struct peeling (use existing code)
            //create new cold struct
            //calculate new indices
          //else continue to next struct

          //for each global in list
            //create new cold global
            //iterate through all GEP
              //if GEP accesses a global (first operand) and source == curr struct and return == curr struct
                //set prec to true
              //if prec is true and source == curr struct and return != curr struct
                //determine whether a cold or hot index was used and make aprropriate changes (use existing code)


namespace {

struct peelAoS : public PassInfoMixin<peelAoS> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {

        errs()<<"\n-------------------------- STRUCT PEELING --------------------------\n\n";
        
        // Since there is no way to get names of each struct element, each element will be uniquely identified by its type and index

        vector<StructType*> allStructs; //stores all structs to be optimised
        vector<pair<GlobalVariable*,StructType*>> globalAoS;
        vector<tuple<Value*,StructType*,Function*>> localAoS;

        /// ONLY COLLECT STRUCTS OF AoS values that:
        // - are global
        // - not used as function parameters
        // - does not contain a ptr element - to make sure it is not recursive

        if(confirmed.size() == 0) //if no AoS is found, do not apply this optimisation
        {
          errs()<<"No AoS values found. Not applying struct peeling.\n";
          errs()<<"\n----------------------- END OF STRUCT PEELING -----------------------\n";
          return PreservedAnalyses::all();
        }
        else
        {
          //get structs used by all AoS
          for(int i = 0; i < confirmed.size(); i++)
          {
            StructType* structure = get<3>(confirmed.at(i));
            Value* aos = get<0>(confirmed.at(i));
            Function* origFunc = get<1>(confirmed.at(i));
            bool isStatic = false;
            if(get<2>(confirmed.at(i)) == "static")
              isStatic = true;
            bool isParam = get<4>(confirmed.at(i));
            bool hasPointerElem = get<5>(confirmed.at(i));

            DataLayout* TD = new DataLayout(&M);

            if(!isParam & !hasPointerElem & isStatic & (TD->getStructLayout(structure)->getSizeInBytes() > 8)) //only optimise structs that have a size greater than a word length (8 bytes)
            {
              if(structure == nullptr) //should not occur
              {
                errs()<<"WARNING: nullptr found!\n";
                continue;
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
              
              if(auto *GV = dyn_cast<GlobalVariable>(aos))
              {
                globalAoS.push_back(make_pair(GV,structure));
              }
              else
              {
                localAoS.push_back(make_tuple(aos,structure,origFunc));
              }
            }
            else
            {
              //erase the structure from globalAoS and localAoS vectors - cannot optimise it - used as function parameter
              for(auto it = globalAoS.begin(); it != globalAoS.end(); it)
              {
                StructType* structToRemove = get<1>(*it);
                if(structToRemove == structure)
                {
                  globalAoS.erase(it);
                }
                else
                {
                  it++;
                }
              }

              for(auto it = localAoS.begin(); it != localAoS.end(); it)
              {
                StructType* structToRemove = get<1>(*it);
                if(structToRemove == structure)
                {
                  localAoS.erase(it);
                }
                else
                {
                  it++;
                }
              }

              for(auto it = allStructs.begin(); it != allStructs.end(); it)
              {
                StructType* structToRemove = *it;
                if(structToRemove == structure)
                {
                  errs()<<"No longer optimising "<<structure->getName()<<"\n";
                  allStructs.erase(it);
                }
                else
                {
                  it++;
                }
              }
            }

          }
        }

          // for(auto it = localAoS.begin(); it != localAoS.end(); it++)
          // {
          //   Value* structToRemove = get<0>(*it);
          //   structToRemove->print(errs());
          // }

        if(allStructs.size() != 0) 
        {
          errs()<<"Optimising "<<globalAoS.size()<<" global AoS and "<<localAoS.size()<<" local AoS, which contain the following struct(s):\n";
        }
        else
        {
          errs()<<"No structs available to optimise. Not applying struct peeling.\n";
          errs()<<"\n----------------------- END OF STRUCT PEELING -----------------------\n";
          return PreservedAnalyses::all(); 
        }

        for(int i = 0; i < allStructs.size(); i++)
        {
          allStructs.at(i)->print(errs());
          errs()<<"\n";

          StructType* currStruct = allStructs.at(i);

          /// stores all global variables that work on the current struct - if detectAoS pass was not run, only optimise any global AoS 
          // if(confirmed.size() == 0)
          // {
          //   for(auto Global = M.global_begin(); Global != M.global_end(); ++Global)
          //   {
          //     Constant* constValue; 
          //     constValue = Global->getInitializer(); //get the initializer

          //     Type* t = constValue->getType(); //get the type of the global
          //     string type_str;
          //     raw_string_ostream tstr(type_str);
          //     t->print(tstr);

          //     Type* globalElem;

          //     if(auto *AT = dyn_cast<ArrayType>(t)) //if global is an array, gets it element type
          //       globalElem = AT->getArrayElementType();

          //     if(currStruct == globalElem) // if element type is equal to current struct, this global is an AoS
          //     {
          //       GlobalVariable* gv = cast<GlobalVariable>(Global);
          //       globalAoS.push_back(make_pair(gv,currStruct));
          //     }
          //     else // otherwise, skip to next global in program
          //       continue;
          //   }
          // }

          // if(globalAoS.size() > 0) // proceed with struct peeling if some global AoS exists using current struct, otherwise continue to next struct
          // {
              vector<float> elems; //this is used to store the no. of times each field element was used in the current struct

              for(int j = 0; j < currStruct->elements().size(); j++) //initialise counts to zero
                elems.push_back(0.0);

              // for each instruction, look for GEP instructions and count the number of times each field of a struct was used
              for (auto &F : M) 
              { 
                for (auto &B : F)  
                {
                  // LoopInfo& loops = getAnalysis<LoopInfoWrapperPass>().getLoopInfo(); //sort this out ltr - check trello

                  //// SORT OUT LATER ////
                  // LoopInfo LI = LoopInfo();
                  // Loop* l = LI.getLoopFor(&B); 
                  // runOnLoop(l); //gives seg fault

                  for (auto &I : B) /// iterate through all instructions
                  {
                    if(auto *GEP = dyn_cast<GetElementPtrInst>(&I)) //for GEP instructions
                    {
                      // only inspect GEP instructions that access the current struct, but does not return the current struct
                      if(GEP->getSourceElementType() == currStruct & GEP->getResultElementType() != currStruct)
                      {
                        string operand_str;
                        raw_string_ostream str(operand_str);
                        Value* index = GEP->getOperand(GEP->getNumIndices()); //get last index value being access
                        index->printAsOperand(str);

                        size_t space_pos = operand_str.find(" ");    
                        string type;
                        string indexStr;

                        // string manipulation to get 'type' and 'index' of the indices
                        if (space_pos != std::string::npos) 
                        {
                          type = operand_str.substr(0, space_pos);
                        }

                        indexStr = operand_str.substr(space_pos + 1);

                        int in = std::stoi(indexStr); // index accessed by GEP instruction

                        /// increment correct index counter 
                        float count = elems.at(in);
                        count++;
                        elems.at(in) = count;
                      }
                    }
                  }
              }
            }

            /// finding mean no. of accesses
            float total = 0.0;
            float mean = 0.0;
            for(int j = 0; j < elems.size(); j++)
            {
              // errs()<<elems.at(j)<<"\n";
              total += elems.at(j);  
            }

            mean = total/elems.size();

            // split fields based on mean no. of accesses
            // with no. of accesses >= mean, put field in hot struct
            // with no. of accesses < mean, put field in cold struct

            ArrayRef<Type*> elemTypes = currStruct->elements(); // get all element types of current struct
            
            /// vectors to store fields and indices for hot and cold structs
            vector<Type*> hotFields;
            vector<pair<int,int>> hotIndices;
            vector<Type*> coldFields;
            vector<pair<int,int>> coldIndices;

            /// put each struct field into a hot or cold struct, based on whether its been accessed frequently (> mean)
            int index = 0;
            int coldIndex = 0;
            int hotIndex = 0;
            for(auto it = elemTypes.begin(); it != elemTypes.end(); it++)
            {
              if(elems.at(index) >= mean)
              {
                hotFields.push_back(*it);
                hotIndices.push_back(make_pair(index,hotIndex));
                hotIndex++;
              }
              else if(elems.at(index) < mean)
              {
                coldFields.push_back(*it);
                coldIndices.push_back(make_pair(index,coldIndex));
                coldIndex++;
              }

              index++;
            }

            if(coldFields.size() == 0) //no cold field required for this struct
            {
              errs()<<"No need to peel this struct - no cold fields found\n";
              break;
            }

            //// create new hot and cold structs
            //// for hot struct, replace fields of current struct, keeping the same name
            //// for cold struct, create new struct with the cold fields, with new name

            ArrayRef<Type*> newHotFields = ArrayRef(hotFields);
            ArrayRef<Type*> newColdFields = ArrayRef(coldFields);
            
            //// creating hot struct
            allStructs.at(i)->setBody(newHotFields);
            
            //// creating cold struct
            string coldName = currStruct->getName().str();
            coldName = coldName.append("Cold");

            StructType* coldStruct = StructType::create(peel_Context, coldName);
            coldStruct->setBody(coldFields);

            //// now for each global AoS, iterate through all GEP instructions that access the global AoS and its struct element and change its:
            //// - indices
            //// - accessed ptr
            //// - source and result element types

            for(int j = 0; j < globalAoS.size(); j++)
            {
              if(globalAoS.at(j).second == currStruct)
              {
                GlobalVariable* curr = globalAoS.at(j).first; //current global AoS

                string global_str; //store global AoS name as a string
                raw_string_ostream str(global_str);
                curr->print(str);

                //// variables required to create a new global AoS to store elements part of the cold struct, making sure it has the same attributes as the original AoS that is now storing hot fields
                Type* globalElem;
                int globalSize;
                MaybeAlign globalAlign;
                string globalName;

                bool isDSOLocal = curr->isDSOLocal();
                GlobalValue::LinkageTypes linkageType = curr->getLinkage();

                Constant* constValue; 
                constValue = curr->getInitializer();    
                Type* t = constValue->getType();

                globalAlign = curr->getAlign();
                globalName = curr->getName().str();
                globalName = globalName.append("Cold");

                if(auto *AT = dyn_cast<ArrayType>(t))
                {
                  globalElem = AT->getArrayElementType();
                  globalSize = AT->getArrayNumElements();
                }

                //// creating new global AoS that has same attributes and size as original AoS, but it now stores the cold struct as elements
                Type* newTy = ArrayType::get(coldStruct,globalSize);
                GlobalVariable* coldArray = new GlobalVariable(M, newTy, false, linkageType, Constant::getNullValue(newTy));
                coldArray->setName(globalName); //set name
                coldArray->setAlignment(globalAlign); //set alignment
                coldArray->setDSOLocal(isDSOLocal); //set this attribute to be same as original

                GetElementPtrInst* prec; //stores preceding GEP inst - the GEP that accesses an AoS element, not the struct fields
                bool precSet = false;
                bool isArray = false;

                //// iterate through all GEP instructions and change indices, ptr operands and source and result element types, based on whether a hot or cold struct was used 
                for(auto &F : M) 
                { 
                  for(auto &B : F)  
                  {
                    for(auto &I : B) /// iterate through all instructions
                    {
                      if(auto *GEP = dyn_cast<GetElementPtrInst>(&I))
                      {
                        Type* op = GEP->getSourceElementType(); // get the type the GEP is accessing from

                        //// get the ptr operand of the GEP - this has to be changed to the cold array if a cold index was used
                        string ptr_string;
                        raw_string_ostream pstr(ptr_string);
                        GEP->getOperand(0)->print(pstr);

                        Value* operand = GEP->getOperand(0);

                        if(auto *LI = dyn_cast<LoadInst>(operand))
                        {
                          Value* ptrOp = LI->getPointerOperand();
                          // ptrOp->print(errs());
                          for (Value::use_iterator i = ptrOp->use_begin(), e = ptrOp->use_end(); i != e; i++)
                          {
                            if(auto *SI = dyn_cast<StoreInst>(i->getUser()))
                            {
                              ptr_string = "";
                              SI->getValueOperand()->print(pstr);
                              if(auto *GEP = dyn_cast<GetElementPtrInst>(operand))
                              {
                                GEP->getOperand(0)->print(pstr);
                              }
                              // errs()<<"yh\n";
                              // errs()<<ptr_string<<"\n";
                              // errs()<<global_str<<"\n";
                            }
                          }
                        }

                        //// if the source element of the GEP is an array, get its element type and number of elements
                        Type* elemType;
                        int numElem;

                        bool condition = false;

                        if(auto *AT = dyn_cast<ArrayType>(op))
                        {
                          elemType = AT->getArrayElementType();
                          numElem = AT->getNumElements();
                          condition = (op->isAggregateType() & (elemType == currStruct));
                          if(precSet == false)
                          {
                            isArray = true;
                          }
                        }
                        else
                        {
                          if(precSet == false)
                          {
                            isArray = false;
                          }
                          condition = op->isStructTy();
                        }

                        if(condition & precSet == false & (global_str == ptr_string)) //if a GEP access to the current global AoS is found, the preceding GEP is found
                        {
                          // GEP->print(errs());
                          // errs()<<"\n";
                          prec = GEP;
                          precSet = true; //flag is set to true, indicating that a global AoS is being accessed and the next GEP instruction accesses the struct fields
                        } 
                        else if(GEP->getSourceElementType() == currStruct & GEP->getResultElementType() != currStruct & precSet == true) //if the GEP that is accessing a struct field is found, following prec
                        {
                          // GEP->print(errs());
                          // errs()<<"\n";
                          string operand_str;
                          raw_string_ostream opstr(operand_str);
                          Value* index = GEP->getOperand(GEP->getNumIndices()); //get last index value being access
                          index->printAsOperand(opstr);

                          size_t space_pos = operand_str.find(" ");    
                          string type;
                          string indexStr;

                          //string manipulation to get 'type' and 'name' of the indices
                          if (space_pos != std::string::npos) 
                          {
                            type = operand_str.substr(0, space_pos);
                          }

                          indexStr = operand_str.substr(space_pos + 1);

                          int in = std::stoi(indexStr); // the index being accessed by the GEP instruction

                          //// determine whether the index now belongs to the hot or cold struct
                          bool isColdIndex = false;
                          bool isHotIndex = false;
                          int newIndex = 0;

                          while(isColdIndex == false & isHotIndex == false)
                          {
                            //// checking if this index matches any of the cold indices
                            for(int j = 0; j < coldIndices.size(); j++)
                            {
                              if(coldIndices.at(j).first == in)
                              {
                                isColdIndex = true;
                                newIndex = coldIndices.at(j).second;
                                break;
                              }
                            }

                            //// checking if this index matches any of the hot indices
                            for(int j = 0; j < hotIndices.size(); j++)
                            {
                              if(hotIndices.at(j).first == in)
                              {
                                isHotIndex = true;
                                newIndex = hotIndices.at(j).second;
                                break;
                              }
                            }
                          }

                          if(isHotIndex & !isColdIndex) // if the GEP operates on a index that should be in a hot struct
                          {
                            if(isArray)
                            {
                              Type* newTy = ArrayType::get(currStruct,numElem);
                              prec->setSourceElementType(newTy); // set this to array of hot structs
                            }
                            else
                              prec->setSourceElementType(currStruct);

                            prec->setResultElementType(currStruct); // set result element type of prec to hot struct
                            GEP->setOperand(GEP->getNumIndices(),ConstantInt::get(peel_Context,APInt(32,newIndex))); // change the index
                            GEP->setSourceElementType(currStruct); // set source element type to hot struct
                          }
                          else if(!isHotIndex & isColdIndex)  // if the GEP operates on a index that should be in a cold struct
                          {
                            if(isArray)
                            {
                              Type* newTy = ArrayType::get(coldStruct,numElem); 
                              prec->setSourceElementType(newTy); // set this to array of cold structs
                            }
                            else
                              prec->setSourceElementType(coldStruct);

                            prec->setResultElementType(coldStruct); // set result element type of prec to cold struct
                            GEP->setOperand(GEP->getNumIndices(),ConstantInt::get(peel_Context,APInt(32,newIndex))); // change the index
                            GEP->setSourceElementType(coldStruct); // set source element type to cold struct
                            prec->setOperand(0,coldArray); //change ptr operand of prec to cold array
                          }
                          precSet = false; // set this to false so next array access GEP can be found
                          isArray = false;
                        }
                        // GEP->print(errs());
                        // errs()<<"\n";
                      }
                    }
                  }
                }
              }
            }
          //} // End bracket for Line 145

          //changing the GEP indices for local AoS 
          for(int j = 0; j < localAoS.size(); j++)
          {
            Value* aos = get<0>(localAoS.at(j));
            string aos_string;
            raw_string_ostream aos_str(aos_string);
            aos->print(aos_str);

            StructType* structure = get<1>(localAoS.at(j));
            Function* origFunc = get<2>(localAoS.at(j));
            // errs()<<"AOS:\n";
            // aos->print(errs());
            // errs()<<"\n";

            if(structure == currStruct) //only optimise structs that have a size greater than a word length (8 bytes)
            {
              if(auto *AI = dyn_cast<AllocaInst>(aos)) //static AoS
              {
                Value* size = AI->getOperand(0);
                string name = AI->getName().str();
                int addressSpace = AI->getAddressSpace();
                name.append("Cold");
                Align alignment = AI->getAlign();

                GetElementPtrInst* prec; //stores preceding GEP inst - the GEP that accesses an AoS element, not the struct fields
                bool precSet = false;
                bool foundBlock = false;

                AllocaInst* coldArray = nullptr;

                //iterate thru orig F and I to see aos exists
                //in that same block, the GEP should exist, given that it is local AoS

                // AllocaInst* coldArray = new AllocaInst(coldStruct,addressSpace,size,alignment,name,&I);
                // coldArray->setName(name);
                // coldArray->setAlignment(alignment);
                // coldArray->print(errs());
                bool isArray = false;

                Instruction* aosLocation = nullptr;

                for(auto &B : *origFunc)  
                {
                  for(auto &I : B) /// iterate through all instructions
                  {
                      // if(I.isSameOperationAs(cast<Instruction>(aos)))
                      // {
                      //   errs()<<"Found aos\n";
                      //   break;
                      // }
                      if(aosLocation == nullptr)
                      {
                        string inst_string;
                        raw_string_ostream inst_str(inst_string);
                        I.print(inst_str);

                        if(aos_string == inst_string)
                        {
                          aosLocation = &I;
                        }

                      }

                      if(auto *GEP = dyn_cast<GetElementPtrInst>(&I))
                      {
                        Type* op = GEP->getSourceElementType(); // get the type the GEP is accessing from

                        //// get the ptr operand of the GEP - this has to be changed to the cold array if a cold index was used
                        string ptr_string;
                        raw_string_ostream pstr(ptr_string);
                        Value* operand = GEP->getOperand(0);

                        if(auto *LI = dyn_cast<LoadInst>(operand))
                        {
                          Value* ptrOp = LI->getPointerOperand();
                          // errs()<<"LOAD FOUND\n";
                          for (Value::use_iterator i = ptrOp->use_begin(), e = ptrOp->use_end(); i != e; ++i)
                          {
                            if(auto *SI = dyn_cast<StoreInst>(i->getUser()))
                            {
                              ptr_string = "";
                              operand = SI->getValueOperand();
                              // errs()<<"yh\n";
                              if(auto *GEP = dyn_cast<GetElementPtrInst>(operand))
                              {
                                operand = GEP->getOperand(0);
                              }
                              // errs()<<ptr_string<<"\n";
                              // errs()<<global_str<<"\n";
                              // errs()<<"STORE FOUND\n";
                            }
                          }
                        }

                        //// if the source element of the GEP is an array, get its element type and number of elements
                        Type* elemType;
                        int numElem;

                        bool condition = false;

                        if(auto *AT = dyn_cast<ArrayType>(op))
                        {
                          elemType = AT->getArrayElementType();
                          numElem = AT->getNumElements();
                          condition = (op->isAggregateType() & (elemType == currStruct));
                          if(precSet == false)
                          {
                            isArray = true;
                          }
                        }
                        else
                        {
                          if(precSet == false)
                          {
                            isArray = false;
                          }
                          condition = op->isStructTy();
                        }

                        if(condition & precSet == false & (aos == operand)) //if a GEP access to the current global AoS is found, the preceding GEP is found
                        {
                          prec = GEP;
                          precSet = true; //flag is set to true, indicating that a global AoS is being accessed and the next GEP instruction accesses the struct fields
                        } 
                        else if(GEP->getSourceElementType() == currStruct & GEP->getResultElementType() != currStruct & precSet == true) //if the GEP that is accessing a struct field is found, following prec
                        {
                          // coldArray->print(errs());
                          string operand_str;
                          raw_string_ostream opstr(operand_str);
                          Value* index = GEP->getOperand(GEP->getNumIndices()); //get last index value being access
                          index->printAsOperand(opstr);

                          size_t space_pos = operand_str.find(" ");    
                          string type;
                          string indexStr;

                          //string manipulation to get 'type' and 'name' of the indices
                          if (space_pos != std::string::npos) 
                          {
                            type = operand_str.substr(0, space_pos);
                          }

                          indexStr = operand_str.substr(space_pos + 1);

                          int in = std::stoi(indexStr); // the index being accessed by the GEP instruction

                          //// determine whether the index now belongs to the hot or cold struct
                          bool isColdIndex = false;
                          bool isHotIndex = false;
                          int newIndex = 0;

                          while(isColdIndex == false & isHotIndex == false)
                          {
                            //// checking if this index matches any of the cold indices
                            for(int j = 0; j < coldIndices.size(); j++)
                            {
                              if(coldIndices.at(j).first == in)
                              {
                                isColdIndex = true;
                                newIndex = coldIndices.at(j).second;
                                break;
                              }
                            }

                            //// checking if this index matches any of the hot indices
                            for(int j = 0; j < hotIndices.size(); j++)
                            {
                              if(hotIndices.at(j).first == in)
                              {
                                isHotIndex = true;
                                newIndex = hotIndices.at(j).second;
                                break;
                              }
                            }
                          }

                          if(coldArray == nullptr & isColdIndex) //only create the cold array if a cold index is being referenced
                          {
                            // IRBuilder<> TmpB(&origFunc->getEntryBlock(),origFunc->getEntryBlock().begin());
                            if(isArray)
                            {
                              coldArray = new AllocaInst(ArrayType::get(coldStruct,numElem),0,size,alignment,name,aosLocation);
                              // new AllocaInst(ArrayType::get(coldStruct,numElem),0,size,alignment,name,aosLocation);
                            }
                            else
                              coldArray = new AllocaInst(coldStruct,0,size,alignment,name,aosLocation);

                            coldArray->setName(name);
                            coldArray->setAlignment(alignment);
                          }

                          if(isHotIndex & !isColdIndex) // if the GEP operates on a index that should be in a hot struct
                          {
                            // errs()<<"HOT\n";
                            if(isArray)
                            {
                              Type* newTy = ArrayType::get(currStruct,numElem);
                              prec->setSourceElementType(newTy); // set this to array of hot structs
                            }
                            else
                              prec->setSourceElementType(currStruct);

                            prec->setResultElementType(currStruct); // set result element type of prec to hot struct
                            GEP->setOperand(GEP->getNumIndices(),ConstantInt::get(peel_Context,APInt(32,newIndex))); // change the index
                            GEP->setSourceElementType(currStruct); // set source element type to hot struct
                          }
                          else if(!isHotIndex & isColdIndex)  // if the GEP operates on a index that should be in a cold struct
                          {
                            // errs()<<"COLD\n";
                            // Type* newTy = ArrayType::get(coldStruct,numElem); 
                            // prec->setSourceElementType(newTy); // set this to array of cold structs
                            if(isArray)
                            {
                              Type* newTy = ArrayType::get(coldStruct,numElem); 
                              prec->setSourceElementType(newTy); // set this to array of cold structs
                            }
                            else
                              prec->setSourceElementType(coldStruct);

                            prec->setResultElementType(coldStruct); // set result element type of prec to cold struct
                            GEP->setOperand(GEP->getNumIndices(),ConstantInt::get(peel_Context,APInt(32,newIndex))); // change the index
                            GEP->setSourceElementType(coldStruct); // set source element type to cold struct
                            prec->setOperand(0,coldArray); //change ptr operand of prec to cold array
                          }
                          precSet = false; // set this to false so next array access GEP can be found
                          isArray = false;
                        }
                        // GEP->print(errs());
                        // errs()<<"\n";
                      }
                  }
                }
              }
            }
          } 
        }

        errs()<<"\n----------------------- END OF STRUCT PEELING -----------------------\n";
        //// Set to ::all() if IR is unchanged, otherwise ::none()
        return PreservedAnalyses::none();
    };
};

}

//Creates plugin for pass - required
// extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
// llvmGetPassPluginInfo() {
//   return {
//     LLVM_PLUGIN_API_VERSION, "passName", "v0.1",
//     [](PassBuilder &PB) {
//       PB.registerPipelineParsingCallback(
//         [](StringRef Name, ModulePassManager &MPM, //For FunctionPass use FunctionPassManager &FPM
//         ArrayRef<PassBuilder::PipelineElement>) {
//           if(Name == "peelAoS"){ //name of pass
//             MPM.addPass(peelAoS());
//             return true;
//           }
//           return false;
//         }
//       );
//     }
//   };
// }
