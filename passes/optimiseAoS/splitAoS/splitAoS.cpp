#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include "../../detectAoS/detectAoS.h"

using namespace llvm;
using namespace std;

LLVMContext split_Context;
IRBuilder<> split_Builder(split_Context);

namespace {

//For Function Pass, use run(Function &F, FunctionAnalysisManager &FM)
struct splitAoS : public PassInfoMixin<splitAoS> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
        //ADD CODE HERE:

        // get vector of <AoS>
        // for each struct
            // find AoS that use current struct and put it in new list
        // if this list size > 0, proceed with struct splitting (use code from peelAoS)
            //create new cold struct
            //make a new struct ptr field in the current struct that points to the cold struct
            //calculate new indices
        // otherwise skip to next struct

            // for each AoS in list
                //iterate through all GEP
                //if GEP accesses a global (first operand) and source == curr struct and return == curr struct
                    //set prec to true
                //if prec is true and source == curr struct and return != curr struct
                    //determine whether a cold or hot index was used and make aprropriate changes (use existing code)
                    //for cold indices, need to add a new GEP to cold struct 
                    //create new load to that ptr, and access new index


        //Sample code to return all function names
        // for (auto &F : M) {
        //     errs() << "I saw a function called " << F.getName() << "!\n";
        // }

        errs()<<"\n-------------------------- STRUCT SPLITTING --------------------------\n\n";

        vector<StructType*> allStructs; //stores all structs to be optimised
        vector<tuple<Value*,StructType*,int>> aosValues; //AoS values to optimise - int is the index of cold struct ptr

        map<StructType*,bool> functionCreated; //stores bool for each struct to shows that a free function was created for it

        float totalColdFieldAccesses = 0.0;

        // vector<pair<GlobalVariable*,StructType*>> globalAoS;
        // vector<tuple<Value*,StructType*,Function*>> localAoS;

        DataLayout* TD = new DataLayout(&M); //to get size and layout of structs

        //check for unique "permitStructSplitting" global variable and see if it exists, before proceed to split struct
        if(permitStructSplitting == false)
        {
          errs()<<"Struct splitting is not permitted for this program. The optimisation may be disabled or has already been applied.\n";
          errs()<<"\n----------------------- END OF STRUCT SPLITTING -----------------------\n";
          return PreservedAnalyses::all();
        }

        /// ONLY COLLECT STRUCTS OF AoS values that satisfy one or multiple of the following criteria:
        // - are used as function arguments
        // - dynamic
        // - contains a ptr element - which might make it a recursive struct

        if(confirmed.size() == 0) //if no AoS is found, do not apply this optimisation (MAYBE GET RID OF THIS, IT IS POSSIBLE TO SPLIT ALL STRUCTS)
        {
          errs()<<"No AoS values found. Not applying struct splitting.\n";
          errs()<<"\n----------------------- END OF STRUCT SPLITTING -----------------------\n";
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
            bool isDynamic = false;
            if(get<2>(confirmed.at(i)) == "dynamic")
              isDynamic = true;
            bool isParam = get<4>(confirmed.at(i));
            bool hasPointerElem = get<5>(confirmed.at(i));

            if((isParam | hasPointerElem | isDynamic) & (TD->getStructLayout(structure)->getSizeInBytes() > 8)) //only optimise structs that have a size greater than a word length (8 bytes)
            {
              if(structure == nullptr) //should not occur
              {
                errs()<<"WARNING: nullptr found!\n";
                continue;
              }
              functionCreated.insert(make_pair(structure,false));
              aosValues.push_back(make_tuple(aos,structure,0));
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
              
              // if(auto *GV = dyn_cast<GlobalVariable>(aos))
              // {
              //   globalAoS.push_back(make_pair(GV,structure));
              // }
              // else
              // {
              //   localAoS.push_back(make_tuple(aos,structure,origFunc));
              // }
            }
            else
            {
              //erase the structure from globalAoS and localAoS vectors - cannot optimise it - used as function parameter
              // for(auto it = globalAoS.begin(); it != globalAoS.end(); it)
              // {
              //   StructType* structToRemove = get<1>(*it);
              //   if(structToRemove == structure)
              //   {
              //     globalAoS.erase(it);
              //   }
              //   else
              //   {
              //     it++;
              //   }
              // }

              functionCreated.erase(structure);

              for(auto it = aosValues.begin(); it != aosValues.end(); it)
              {
                Value* AoSToRemove = get<0>(*it);
                if(AoSToRemove == aos)
                {
                  aosValues.erase(it);
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

        if(allStructs.size() != 0 & aosValues.size() != 0) 
        {
          // errs()<<"Optimising "<<globalAoS.size()<<" global AoS and "<<localAoS.size()<<" local AoS, which contain the following struct(s):\n";
          errs()<<"Optimising "<<aosValues.size()<<" AoS data structures with the following struct(s):\n";
        }
        else
        {
          errs()<<"No structs available to optimise. Not applying struct splitting.\n";
          errs()<<"\n----------------------- END OF STRUCT SPLITTING -----------------------\n";
          return PreservedAnalyses::all(); 
        }
        

        for(int i = 0; i < allStructs.size(); i++)
        {
          allStructs.at(i)->print(errs());
          errs()<<"\n";
          
          StructType* currStruct = allStructs.at(i);

          //Need to identify if the struct contains a recursive ptr to itself

          vector<float> elems; //this is used to store the no. of times each field element was used in the current struct

          vector<int> pointerIndices;
          vector<int> recursivePointerIndices;

          int inspectingIndex = -1;

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

              bool searchForStore = false;

              for (auto &I : B) /// iterate through all instructions
              {
                if(searchForStore)
                {
                  if(auto *SI = dyn_cast<StoreInst>(&I))
                  {
                    Value* elem = SI->getValueOperand();

                    //check whether this operand is a struct alloca, or a AoS present in 'confirmed' list
                    if(auto *AI = dyn_cast<AllocaInst>(elem))
                    {
                      AI->print(errs());
                      errs()<<"\n";
                      // AI->getAllocatedType()->print(errs());
                      // errs()<<"\n";
                      if(AI->getAllocatedType() == currStruct) //single struct allocation
                      {
                        recursivePointerIndices.push_back(inspectingIndex);
                        elems.at(inspectingIndex) = INT_MAX;
                        errs()<<"Found recursive ptr at index "<<inspectingIndex<<"\n";
                      }
                      // else if(auto *Array = dyn_cast<ArrayType>(AI->getAllocatedType())) //static AoS allocation
                      // {
                      //   if(Array->getArrayElementType() == currStruct)
                      //   {
                      //     recursivePointerIndices.push_back(inspectingIndex);
                      //     errs()<<"Found recursive static AoS ptr at index "<<inspectingIndex<<"\n";
                      //   }
                      // }
                    }
                    else if(auto *LI = dyn_cast<LoadInst>(elem))
                    {
                      Value* loadedElem = LI->getPointerOperand();
                      for(int j = 0; j < confirmed.size(); j++)
                      {
                        Value* aos = get<0>(confirmed.at(j));
                        if(loadedElem == aos & (get<3>(confirmed.at(j)) == currStruct))
                        {
                          recursivePointerIndices.push_back(inspectingIndex);
                          elems.at(inspectingIndex) = INT_MAX;
                          errs()<<"Found recursive AoS ptr at index "<<inspectingIndex<<"\n";
                          break;
                        }
                      }
                    }
                    else if(auto *GEP = dyn_cast<GetElementPtrInst>(elem))
                    {
                      Value* sourceElem = GEP->getPointerOperand();
                      for(int j = 0; j < confirmed.size(); j++)
                      {
                        Value* aos = get<0>(confirmed.at(j));
                        if(sourceElem == aos & (get<3>(confirmed.at(j)) == currStruct))
                        {
                          recursivePointerIndices.push_back(inspectingIndex);
                          elems.at(inspectingIndex) = INT_MAX;
                          errs()<<"Found recursive AoS ptr at index "<<inspectingIndex<<"\n";
                          break;
                        }
                      }
                    }

                    searchForStore = false;
                    inspectingIndex = -1;
                  }
                }
                else if(auto *GEP = dyn_cast<GetElementPtrInst>(&I)) //for GEP instructions
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

                    if(GEP->getResultElementType()->isPointerTy() & elems.at(in) != INT_MAX)
                    {
                      pointerIndices.push_back(in);
                      searchForStore = true;
                      inspectingIndex = in;
                    }
                  }
                }
              }
          }
        }

        // /// printing out all recursive pointer fields in struct
        // for(int j = 0; j < recursivePointerIndices.size(); j++)
        // {
        //   errs()<<"Index "<<recursivePointerIndices.at(i)<<" is recursive\n";
        // }

        /// finding mean no. of accesses
        float total = 0.0;
        float mean = 0.0;
        for(int j = 0; j < elems.size(); j++)
        {
          errs()<<"Field "<<j<<" accessed "<<elems.at(j)<<" times\n";
          if(elems.at(j) != INT_MAX)
            total += elems.at(j);
        }

        mean = total/(elems.size() - recursivePointerIndices.size());
        errs()<<"Mean: "<<mean<<"\n";

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
          for(auto it = aosValues.begin(); it != aosValues.end(); it)
          {
            StructType* structToRemove = get<1>(*it);
            if(structToRemove == currStruct)
              aosValues.erase(it);
            else
              it++;
          }
          continue;
        }

        //// create new hot and cold structs
        //// for hot struct, replace fields of current struct, keeping the same struct name, and add a new ptr field to the cold struct
        //// for cold struct, create new struct with the cold fields, with new struct name

        ////// ADD POINTER TO COLD STRUCT WITHIN HOT STRUCT

        ArrayRef<Type*> newColdFields = ArrayRef(coldFields);
        
        // //// creating hot struct
        // allStructs.at(i)->setBody(newHotFields);
        
        //// creating cold struct
        string coldName = currStruct->getName().str();
        coldName = coldName.append("Cold");
        StructType* coldStruct = StructType::create(split_Context, coldName);
        coldStruct->setBody(coldFields);

        coldStructs.push_back(coldStruct);

        PointerType* coldStructPtr = PointerType::get(coldStruct, 0); // Initialise the pointer type now
        int coldStructPtrIndex = hotIndex++;
        errs()<<"Cold struct accessed at index: "<<coldStructPtrIndex<<"\n";
        //store coldStructPtrIndex
        for(int s = 0; s < aosValues.size(); s++)
        {
          StructType* structure = get<1>(aosValues.at(s));
          if(structure == currStruct)
            get<2>(aosValues.at(s)) = coldStructPtrIndex;
        }

        // //// creating hot struct
        hotFields.push_back(coldStructPtr);
        ArrayRef<Type*> newHotFields = ArrayRef(hotFields);
        allStructs.at(i)->setBody(newHotFields);

        errs()<<"Hot Struct:\n";
        currStruct->print(errs());
        errs()<<"\n";
        for(int j = 0; j < hotIndices.size(); j++)
        {
          errs()<<"Old index: "<<hotIndices.at(j).first<<" --> New index: "<<hotIndices.at(j).second<<"\n";
        }
        errs()<<"\nCold Struct:\n";
        coldStruct->print(errs());
        errs()<<"\n";

        for(int j = 0; j < coldIndices.size(); j++)
        {
          errs()<<"Old index: "<<coldIndices.at(j).first<<" --> New index: "<<coldIndices.at(j).second<<"\n";
          totalColdFieldAccesses += elems.at(coldIndices.at(j).first);
        }

        errs()<<"Total cold field accesses: "<<totalColdFieldAccesses<<"\n";

        if(totalColdFieldAccesses == 0.0)
        {
          errs()<<"No structure splitting required for this struct: cold fields are not accessed.\n";
          for(auto it = aosValues.begin(); it != aosValues.end(); it)
          {
            StructType* structToRemove = get<1>(*it);
            if(structToRemove == currStruct)
              aosValues.erase(it);
            else
              it++;
          }
          continue;
        }

        // //change GEP

        //when an access to a field that now belongs to the cold struct is found within a block, perform (in this order):
        //1. create GEP inst to access the cold struct ptr
        //2. create new malloc call inst to allocate data
        //3. create store inst to store the returned malloc memory to the GEP 
        //4. create a load inst to the GEP as a ptr, so the pointer operand and the indices of existing GEP inst can be changed for the cold fields

        GetElementPtrInst* prec; //stores preceding GEP inst - the GEP that accesses an AoS element, not the struct fields
        GetElementPtrInst* coldGEP; //need to create a GEP inst to access the cold pointer struct
        bool precSet = false;
        bool isArray = false;

        bool isMalloced = false;
        bool temp = false;

            //// iterate through all GEP instructions and change indices, ptr operands and source and result element types, based on whether a hot or cold struct was used 
        for(auto &F : M) 
        { 
          for(auto &B : F)  
          {
            coldGEP = nullptr;
            for(auto &I : B) /// iterate through all instructions
            {
              /// below works for splitAoS only
              // if(F.getName() == "main")
              // {
              //   if(auto *CI = dyn_cast<CallInst>(&I))
              //   {
              //     if(CI->getCalledFunction()->getName() == "malloc")
              //       temp = true;
              //   }
              //   else
              //   {
              //     if(temp)
              //     {
              //   IRBuilder<> TmpB(&I);
              //   // Instruction* loadInst = new LoadInst(PointerType::get(split_Context,0),coldGEP,"",&I);
              //   TmpB.CreateCall(M.getFunction("malloc_usable_size"),get<0>(confirmed.at(0)),"",nullptr);
              //   temp = false;
              //     }
              //   }
              // }
              if(auto *GEP = dyn_cast<GetElementPtrInst>(&I))
              {
                Type* op = GEP->getSourceElementType(); // get the type the GEP is accessing from

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

                if(condition & precSet == false) //if a GEP access to the current global AoS is found, the preceding GEP is found
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

                  // errs()<<"hot:"<<isHotIndex<<" or cold:"<<isColdIndex<<"\n";
                  // errs()<<"old index:"<<in<<" new index:"<<newIndex<<"\n";

                  if(isHotIndex & !isColdIndex) // if the GEP operates on a index that should be in a hot struct
                  {
                    // if(isArray)
                    // {
                    //   Type* newTy = ArrayType::get(currStruct,numElem);
                    //   prec->setSourceElementType(newTy); // set this to array of hot structs
                    // }
                    // else
                    //   prec->setSourceElementType(currStruct);

                    // prec->setResultElementType(currStruct); // set result element type of prec to hot struct
                    GEP->setOperand(GEP->getNumIndices(),ConstantInt::get(split_Context,APInt(32,newIndex))); // change the index
                    // GEP->setSourceElementType(currStruct); // set source element type to hot struct
                  }
                  else if(!isHotIndex & isColdIndex)  // if the GEP operates on a index that should be in a cold struct
                  {
                    // if(coldGEP == nullptr)
                    // {
                      vector<Value*> indexVector;
                      indexVector.push_back(ConstantInt::get(split_Context,APInt(32,0)));
                      indexVector.push_back(ConstantInt::get(split_Context,APInt(32,coldStructPtrIndex)));
                      ArrayRef<Value*> indexList = ArrayRef(indexVector);
                      coldGEP = GetElementPtrInst::CreateInBounds(currStruct, prec, indexList, "cold", &I);
                      // errs()<<"RET:";
                      // coldGEP->getResultElementType()->print(errs());
                      //                       errs()<<"\n";

                      // coldGEP->print(errs());
                      // errs()<<"\n";

                      // // create alloca inst for cold struct ptr - doesn't work
                      // if(isMalloced == false)
                      // {
                      // Instruction* allocaInst = new AllocaInst(coldStruct,0,ConstantInt::get(split_Context,APInt(32,1)),"",&I);
                      // allocaInst->print(errs());
                      // errs()<<"\n";

                      // Instruction* storeInst = new StoreInst(allocaInst,coldGEP,&I);
                      // storeInst->print(errs());
                      // errs()<<"\n";
                      // isMalloced = true;
                      // }

                      //find a way to create a malloc() call that works without seg fault

                      if(isMalloced == false)
                      {
                        IRBuilder<> TmpB(&I);
                        int size = TD->getStructLayout(coldStruct)->getSizeInBytes();
                        Value* structSize = ConstantInt::get(M.getContext(),APInt(64,size));
                        Type* ITy = Type::getInt32Ty(split_Context);
                        // Instruction* mallocInst = TmpB.CreateMalloc(ITy, coldStruct, structSize,ConstantInt::get(split_Context,APInt(32,1)), nullptr, ""); //creates tail call
                        //above tail call to malloc() should be converted to non-tail call in detectAoS()
                        //it is converting but still giving seg fault - recreating malloc
                        CallInst* mallocInst = TmpB.CreateCall(M.getFunction("malloc"),structSize,"",nullptr);
                        // mallocInst->setAttributes(mallocAttributes);
                        

                        Instruction* storeInst = new StoreInst(mallocInst,coldGEP,&I);
                        // storeInst->print(errs());
                        // errs()<<"\n";
                        isMalloced = true;
                      }

                    //}

                    /* with a coldGEP created/existing, edit the current GEP to access the cold struct by
                      - changing the sourceElementptr to cold struct
                      - changing the ptr operand to the coldGEP
                      - changing the index to the new index
                    */

                    Instruction* loadInst = new LoadInst(PointerType::get(split_Context,0),coldGEP,"",&I);

                    GEP->setSourceElementType(coldStruct);
                    GEP->setOperand(0,loadInst);
                    GEP->setOperand(GEP->getNumIndices(),ConstantInt::get(split_Context,APInt(32,newIndex))); // change the index
                    

                    // if(isArray)
                    // {
                    //   Type* newTy = ArrayType::get(coldStruct,numElem); 
                    //   prec->setSourceElementType(newTy); // set this to array of cold structs
                    // }
                    // else
                    //   prec->setSourceElementType(coldStruct);

                    // prec->setResultElementType(coldStruct); // set result element type of prec to cold struct
                    // GEP->setOperand(GEP->getNumIndices(),ConstantInt::get(peel_Context,APInt(32,newIndex))); // change the index
                    // GEP->setSourceElementType(coldStruct); // set source element type to cold struct
                    // prec->setOperand(0,coldArray); //change ptr operand of prec to cold array
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

        // create a variable in main() to signify that struct splitting has been applied to this program/IR. 
        // This is checked for before struct splitting is applied to it again, which is not permitted.

        // Value* structSplittingApplied = ConstantInt::get(split_Context,APInt(1,0), 1); //initialise index to 0

        //Create a unique global variable with these attributes
        //Similarly a programmer can include a global variable with the same name as "permitStructSplittingFlag" to disable this struct splitting optimisation for any reason.
        GlobalVariable* permitStructSplitting = new GlobalVariable(M, Type::getInt1Ty(split_Context), true, GlobalValue::LinkageTypes::PrivateLinkage, Constant::getNullValue(Type::getInt1Ty(split_Context)), "permitStructSplittingFlag");

        //now for every AoS used as function arguments, create a for/whileloop that frees the allocated cold pointer struct (if it is not NULL)
        //to determine where to create this for/while loop, get the LAST use of the AoS, and add the for/while loop in the next block?

        for(int i = 0; i < aosValues.size(); i++)
        {
          Value* currentAoS = get<0>(aosValues.at(i));
          StructType* currentStruct = get<1>(aosValues.at(i));
          int coldStructPtrIndex = get<2>(aosValues.at(i));
          bool needToCreateFunc = false;
          auto findElem = functionCreated.find(currentStruct);
          if(findElem != functionCreated.end())
            needToCreateFunc = findElem->second;

          errs()<<"need to create func for"<<currentStruct->getName()<<": "<<needToCreateFunc<<"\n";

          BasicBlock* blockToAdd = nullptr;

          Instruction* terminator = nullptr; //point where the new instructions are being added

          LoadInst* aosLoadInst = nullptr;

          bool freeAoSExists = false;

          //eliminate any calls to free() for this AoS

          Function* freeFunc = M.getFunction("free");
          auto usersList = freeFunc->users();
          for(auto usr = usersList.begin(); usr != usersList.end(); usr++)
          {
            Value* call = *usr;
            // call->print(errs());
            // errs()<<"\n";
            if(auto *CI = dyn_cast<CallInst>(call))
            {
              Value* operand = CI->getArgOperand(0);
              if(auto *LI = dyn_cast<LoadInst>(operand))
              {
                if(LI->getPointerOperand() == currentAoS)
                {
                  // if(CI->getNumUses() == 0)
                    // CI->eraseFromParent(); //erase this free() call
                  
                  blockToAdd = CI->getParent();
                  // blockToAdd->printAsOperand(errs());
                  terminator = LI->getNextNode();
                  aosLoadInst = LI;
                  freeAoSExists = true;
                  // errs()<<"\n";
                }
              }
            }
          }
          

          User* lastUsage = *(currentAoS->users().begin()); //last user of the AoS
          // lastUsage->print(errs());
          // errs()<<"\n";

          Instruction* lastInstructionUsage = cast<Instruction>(lastUsage);
          // lastInstructionUsage->printAsOperand(errs());
          // errs()<<" in function: ";
          // lastInstructionUsage->getFunction()->printAsOperand(errs());
          // errs()<<"\n";

          BasicBlock* currentBlock = lastInstructionUsage->getParent(); 
          currentBlock->printAsOperand(errs());
          errs()<<" in function: ";
          currentBlock->getParent()->printAsOperand(errs());
          errs()<<"\n";
          
          if(blockToAdd == nullptr)
            blockToAdd = currentBlock->getSingleSuccessor(); //this may break for blocks with return statements - test it out
          if(blockToAdd == nullptr)
          {
            // errs()<<"No successor\n";
            auto inst = currentBlock->getTerminator();
            // inst->print(errs());
            errs()<<"\n";
            if(auto *BR = dyn_cast<BranchInst>(inst))
            {
              blockToAdd = BR->getSuccessor(BR->getNumSuccessors() - 1);
            }
          }

          // if(blockToAdd == nullptr)
          //   blockToAdd = currentBlock;

          // // blockToAdd->printAsOperand(errs());
          // if(terminator == nullptr)
          //   terminator = blockToAdd->getFirstNonPHI();

          // terminator->print(errs());
          errs()<<"\n";

          IRBuilder<> FreeBuilder(terminator);

          string funcName = "freeAoS";
          funcName.append(currentStruct->getName());

          // Function* aosFreeFunction = M.getFunction("freeAoS");

          if(!needToCreateFunc)
          {
          vector<Type *> ArgTypes;

          ArgTypes.push_back(PointerType::get(M.getContext(),0)); //AoS parameter
          // ArgTypes.push_back(Type::getInt32Ty(split_Context)); //size parameter

          FunctionType *FT = FunctionType::get(Type::getVoidTy(M.getContext()), ArgTypes, false);

          Function* freeAoSFunction = Function::Create(FT,Function::ExternalLinkage,funcName,M);
          // freeAoSFunction->setName(funcName);
          // M.getFunctionList().push_back(freeAoSFunction);

          BasicBlock *entryBlock = BasicBlock::Create(split_Context, "entry", freeAoSFunction);

          // //at entry block, obtain the size of AoS
          FreeBuilder.SetInsertPoint(entryBlock);

          Type* argTy = M.getFunction("malloc_usable_size")->getArg(0)->getType();
          // // Function* mallocFunc = M.getFunction("malloc_usable_size");
          // // Value* a = *(mallocFunc->users().begin());
          // // CallInst* c = cast<CallInst>(a);
          // // a->print(errs());
          // // errs()<<"\n";
          // // argTy = c->getArgOperand(0)->getType();
          // // auto list = c->getAttributes();
          // // auto list = M.getFunction("malloc_usable_size")->getAttributes();
          // // argTy->print(errs());
          // // errs()<<"\n";

          //loading the function argument: aos ptr

          AllocaInst* aosStore = FreeBuilder.CreateAlloca(PointerType::get(split_Context,0), 0, "aos"); //stores aos pointer
          FreeBuilder.CreateStore(freeAoSFunction->getArg(0), aosStore);

          AllocaInst* sizeStore = FreeBuilder.CreateAlloca(Type::getInt32Ty(split_Context), 0, "size"); //stores number of AoS elements
          // FreeBuilder.CreateStore(freeAoSFunction->getArg(1), sizeStore);

          AllocaInst* indexStore = FreeBuilder.CreateAlloca(Type::getInt32Ty(split_Context), 0, "index"); //stores while loop index
          FreeBuilder.CreateStore(ConstantInt::get(split_Context,APInt(32,0)), indexStore); //initialise index to 0

          LoadInst* aosLoad = new LoadInst(argTy,aosStore,"",entryBlock);

          CallInst* mallocSizeCall = FreeBuilder.CreateCall(M.getFunction("malloc_usable_size"),aosLoad,"",nullptr);
          // mallocSizeCall->setAttributes(list);

          // //create variable to store number of elements in AoS
          // Instruction* numElements = new AllocaInst(Type::getInt32Ty(split_Context),0,ConstantInt::get(split_Context,APInt(32,1)),"",terminator);

          //get size of struct used by the AoS
          // int elementSize = TD->getStructLayout(currentStruct)->getSizeInBytes(); //may be incorrect after struct field reordering
          int elementSize = 0;
          if(origStructSizes.find(currentStruct) != origStructSizes.end())
            elementSize = origStructSizes.find(currentStruct)->second; //get the original struct size before any optimisations were applied to it

          Value* elementSizeValue = ConstantInt::get(split_Context,APInt(32,elementSize));

          ///divide the size of AoS by element size to get the number of elements in AoS, store it in numElements

          //divide operation

          // mallocSizeCall->getType()->print(errs());
          // errs()<<"\n";
          // elementSizeValue->getType()->print(errs());
          // errs()<<"\n";

          // if(mallocSizeCall->getType() == elementSizeValue->getType())
          //   errs()<<"yes\n";

          Instruction* trunc = new TruncInst(mallocSizeCall,Type::getInt32Ty(split_Context),"",entryBlock);

          Value* result = FreeBuilder.CreateUDiv(trunc,elementSizeValue,"");

          Instruction* storeSize = new StoreInst(result,sizeStore,entryBlock);
          
          //creating while.cond block

          BasicBlock *whileCond = BasicBlock::Create(split_Context, "while.cond", freeAoSFunction);
          
          FreeBuilder.CreateBr(whileCond);
          /// End of entry block

          /// Start of whileCond block

          FreeBuilder.SetInsertPoint(whileCond);

          LoadInst* indexLoad = FreeBuilder.CreateLoad(Type::getInt32Ty(split_Context),indexStore); //load index
          LoadInst* sizeLoad = FreeBuilder.CreateLoad(Type::getInt32Ty(split_Context),sizeStore); //load size

          Value* whileConditon = FreeBuilder.CreateICmp(ICmpInst::ICMP_NE,indexLoad,sizeLoad,"condition");

          //creating whileBody block

          BasicBlock* whileBody = BasicBlock::Create(split_Context, "while.body", freeAoSFunction);

          //creating whileEnd block

          BasicBlock *whileEnd = BasicBlock::Create(split_Context, "while.end", freeAoSFunction);

          FreeBuilder.CreateCondBr(whileConditon,whileBody,whileEnd);

          /// End of whileCond

          /// Start of whileEnd

          FreeBuilder.SetInsertPoint(whileEnd);
          FreeBuilder.CreateRetVoid(); // end of function

          /// End of whileEnd

          /// Start of whileBody

          FreeBuilder.SetInsertPoint(whileBody);

          aosLoad = FreeBuilder.CreateLoad(argTy,aosStore,"aos"); //load aos to be accessed
          indexLoad = FreeBuilder.CreateLoad(Type::getInt32Ty(split_Context),indexStore); //load index

          //extend index from i32 to i64

          Value* extendedIndex = FreeBuilder.CreateSExt(indexLoad,Type::getInt64Ty(split_Context));

          Value* elemAccess = FreeBuilder.CreateGEP(currentStruct,aosLoad,extendedIndex,"elemAccess",true);

          vector<Value*> indices;
          indices.push_back(ConstantInt::get(split_Context,APInt(32,0)));
          indices.push_back(ConstantInt::get(split_Context,APInt(32,coldStructPtrIndex)));

          Value* coldStructPtrAccess = FreeBuilder.CreateGEP(currentStruct,elemAccess,indices,"coldStructPtr",true);

          LoadInst* gepLoad = FreeBuilder.CreateLoad(coldStructPtrAccess->getType(),coldStructPtrAccess,"coldStruct");

          // Value* ifCondition = FreeBuilder.CreateIsNotNull(gepLoad,"isNull");

          // //creating ifThen and ifEnd

          // BasicBlock *ifThen = BasicBlock::Create(split_Context, "if.then", freeAoSFunction);

          // BasicBlock *ifEnd = BasicBlock::Create(split_Context, "if.end", freeAoSFunction);

          // FreeBuilder.CreateCondBr(ifCondition,ifEnd,ifThen);

          /// End of whileBody

          /// Start of ifEnd

          // FreeBuilder.SetInsertPoint(ifEnd);

          // indexLoad = FreeBuilder.CreateLoad(Type::getInt32Ty(split_Context),indexStore); //load index
          // Value* inc = FreeBuilder.CreateAdd(indexLoad,ConstantInt::get(split_Context,APInt(32,1)),"",false,true);
          // FreeBuilder.CreateStore(inc,indexStore);
          // FreeBuilder.CreateBr(whileCond);

          /// End of ifEnd

          /// Start of ifThen

          // FreeBuilder.SetInsertPoint(ifThen);
          // aosLoad = FreeBuilder.CreateLoad(argTy,aosStore,"aos"); //load aos to be accessed
          // indexLoad = FreeBuilder.CreateLoad(Type::getInt32Ty(split_Context),indexStore); //load index

          // //extend index from i32 to i64

          // extendedIndex = FreeBuilder.CreateSExt(indexLoad,Type::getInt64Ty(split_Context));

          // elemAccess = FreeBuilder.CreateGEP(currentStruct,aosLoad,extendedIndex,"elemAccess",true);

          // indices.clear();
          // indices.push_back(ConstantInt::get(split_Context,APInt(32,0)));
          // indices.push_back(ConstantInt::get(split_Context,APInt(32,coldStructPtrIndex)));

          // coldStructPtrAccess = FreeBuilder.CreateGEP(currentStruct,elemAccess,indices,"coldStructPtr",true);

          // gepLoad = FreeBuilder.CreateLoad(coldStructPtrAccess->getType(),coldStructPtrAccess,"coldStruct");

          Value* freeColdStruct = FreeBuilder.CreateCall(M.getFunction("free"),gepLoad,"");
          // FreeBuilder.CreateBr(ifEnd);

          indexLoad = FreeBuilder.CreateLoad(Type::getInt32Ty(split_Context),indexStore); //load index
          Value* inc = FreeBuilder.CreateAdd(indexLoad,ConstantInt::get(split_Context,APInt(32,1)),"",false,true);
          FreeBuilder.CreateStore(inc,indexStore);

          FreeBuilder.CreateBr(whileCond);

          /// End of ifThen

                    // canBeFreed()  <- probably use this somewhere - use this on the currentAoS Value*

          functionCreated.find(currentStruct)->second = true;
          errs()<<"Created function: "<<funcName<<"\n";
          }

          FreeBuilder.SetInsertPoint(terminator);

          if(aosLoadInst == nullptr)
            aosLoadInst = new LoadInst(currentAoS->getType(),currentAoS,"",terminator);

          CallInst* freeAoSCall = FreeBuilder.CreateCall(M.getFunction(funcName),aosLoadInst,"",nullptr);

          // if(freeAoSExists == false)
          // {
          //   CallInst* freeAoSCall = FreeBuilder.CreateCall(freeFunc,aosLoadInst,"",nullptr);
          // }
          
          
        }

        

        errs()<<"\n----------------------- END OF STRUCT SPLITTING -----------------------\n";


        //Set to ::all() if IR is unchanged, otherwise ::none()
        return PreservedAnalyses::none();
    };
};

}


// //Creates plugin for pass - required
// extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
// llvmGetPassPluginInfo() {
//   return {
//     LLVM_PLUGIN_API_VERSION, "splitAoS", "v0.1",
//     [](PassBuilder &PB) {
//       PB.registerPipelineParsingCallback(
//         [](StringRef Name, ModulePassManager &MPM, //For FunctionPass use FunctionPassManager &FPM
//         ArrayRef<PassBuilder::PipelineElement>) {
//           if(Name == "splitAoS"){ //name of pass
//             MPM.addPass(splitAoS());
//             return true;
//           }
//           return false;
//         }
//       );
//     }
//   };
// }