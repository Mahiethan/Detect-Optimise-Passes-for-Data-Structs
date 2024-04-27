// OPTIMISATION: Structure Splitting

/* Applied to: 
    - locally declared, dynamic AoS
    - AoS that uses structs with pointers
*/

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

struct splitAoS : public PassInfoMixin<splitAoS> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {

        errs()<<"\n-------------------------- STRUCT SPLITTING --------------------------\n\n";

        bool exit = false;

        if(M.getFunction("malloc") == nullptr)
        {
          errs()<<"Function malloc() not found in IR. Unable to perform structure splitting without this function.\n\n";
          errs()<<"Please add this function to the source file.\n\n";
          exit = true;
        }

         if(M.getFunction("free") == nullptr)
        {
          errs()<<"Function free() not found in IR. Unable to perform structure splitting without this function.\n\n";
          errs()<<"Please add this function to the source file.\n";
          exit = true;
        }

        if(exit)
        {
          errs()<<"\n----------------------- END OF STRUCT SPLITTING -----------------------\n";
          return PreservedAnalyses::all();
        }

        vector<StructType*> allStructs; // stores all structs to be optimised
        vector<tuple<Value*,StructType*,int,Value*>> aosValues; // AoS values to optimise - int is the index of cold struct ptr, last Value* is the size of malloc()

        map<StructType*,bool> functionCreated; //stores bool for each struct to shows that a free function was created for it

        float totalColdFieldAccesses = 0.0;

        DataLayout* TD = new DataLayout(&M); // to get size and layout of structs

        // check for unique "permitStructSplitting" global variable and see if it exists, before proceed to split struct
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

        if(confirmed.size() == 0) //if no AoS is found, do not apply this optimisation
        {
          errs()<<"No AoS values found. Not applying struct splitting.\n";
          errs()<<"\n----------------------- END OF STRUCT SPLITTING -----------------------\n";
          return PreservedAnalyses::all();
        }
        else
        {
          /* get structs used by all AoS */

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

            if((isParam | hasPointerElem | isDynamic) & (TD->getTypeAllocSize(structure) > 8)) // only optimise structs that have a size greater than a word length (8 bytes)
            {
              if(structure == nullptr) // should not occur
              {
                errs()<<"WARNING: nullptr found!\n";
                continue;
              }
              functionCreated.insert(make_pair(structure,false));
              aosValues.push_back(make_tuple(aos,structure,0,nullptr));
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
            else
            {

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
          /* Calculate hotness values and affinity groups for each struct */

          allStructs.at(i)->print(errs());
          errs()<<"\n";
          
          StructType* currStruct = allStructs.at(i);

          set<vector<int>> fieldAccessPattern;

          // Need to identify if the struct contains a recursive ptr to itself

          vector<float> elems; // this is used to store the no. of times each field element was used in the current struct

          vector<int> pointerIndices;
          vector<int> recursivePointerIndices;

          int inspectingIndex = -1;

          for(int j = 0; j < currStruct->elements().size(); j++) //initialise counts to zero
            elems.push_back(0.0);

          // for each instruction, look for GEP instructions and count the number of times each field of a struct was used
          // also obtain the affinity groups - groups of fields accessed together

          for (auto &F : M) 
          { 
            for (auto &B : F)  
            {
              vector<int> accessPattern;

              for(int f = 0; f < currStruct->getStructNumElements(); f++)
              {
                accessPattern.push_back(0);
              }

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

                      if(AI->getAllocatedType() == currStruct) //single struct allocation
                      {
                        recursivePointerIndices.push_back(inspectingIndex);
                        elems.at(inspectingIndex) = INT_MAX;
                        errs()<<"Found recursive ptr at index "<<inspectingIndex<<"\n";
                      }
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
                    accessPattern.at(in) = 1;

                    if(GEP->getResultElementType()->isPointerTy() & elems.at(in) != INT_MAX)
                    {
                      pointerIndices.push_back(in);
                      searchForStore = true;
                      inspectingIndex = in;
                    }
                  }
                }
              }

              bool allZero = true;
              for(int f = 0; f < accessPattern.size(); f++)
              {
                if(accessPattern.at(f) != 0)
                {
                  allZero = false;
                  break;
                }
              }

              if(!allZero)
              {
                fieldAccessPattern.insert(accessPattern);
              }
             
          }
        }

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
        map<int,int> usedAsHotIndices;
        vector<Type*> coldFields;
        vector<pair<int,int>> coldIndices;

        map<int,int> hotIndicesForInspection;

        /// put each struct field into a hot or cold struct, based on whether its been accessed frequently (> mean)
        int index = 0;

        int hotIndex = 0;
        for(auto it = elemTypes.begin(); it != elemTypes.end(); it++)
        {
          if(elems.at(index) >= mean)
          {
            hotFields.push_back(*it);
            hotIndices.push_back(make_pair(index,hotIndex));
            usedAsHotIndices.insert(make_pair(index,hotIndex));
            hotIndex++;
            hotIndicesForInspection.insert(make_pair(index,0));
          }

          index++;
        }

        /// START OF AFFINITY GROUP CHECK

        // iterate through the access patterns in 'fieldAccessPattern' vector
        // if a group exists where all fields are accessed, this shows good affinity within the struct - NO need to split

        vector<int> analyse;

        for(int k = 0; k < currStruct->getStructNumElements(); k++)
        {
          analyse.push_back(0);
        }

        bool exitIfAllOnes = false;

        for(auto k = fieldAccessPattern.begin(); k != fieldAccessPattern.end(); k++)
        {
          vector<int> pattern = *k;
          bool shared = false;
          bool allOnes = true;
          for(int j = 0; j < pattern.size(); j++)
          {
            if(pattern.at(j) == 0)
              allOnes = false;
            if(hotIndicesForInspection.find(j) != hotIndicesForInspection.end())
            {
              if(pattern.at(j) == 1)
                shared = true;
            }
            else // if a cold field is found inside an affinity group sharing an access with a hot field, increment the value of analyse
            {
              if(pattern.at(j) == 1)
              {
                if(shared)
                  analyse.at(j)++;
              }
            }

          }

          if(allOnes)
          {
            exitIfAllOnes = true;
            break;
          }
        }

        if(exitIfAllOnes)
        {
          errs()<<"Struct shows good affinity. No need to split it\n"; //if access pattern is all 1s, it means that all fields of the struct are accessed together
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

        /* Separate fields into 'hot' and 'cold' structs based on hotness and affinity */


        map<int,int> toAddToHot; // used as a set data structure - to store unique elements and for easier finding

        for(int k = 0; k < currStruct->getStructNumElements(); k++)
        {
          if(analyse.at(k) > 1)
          {
            toAddToHot.insert(make_pair(k,0));
            errs()<<"Adding field "<<k<<" to the hot struct\n";
          }
        }

        index = 0;
        int coldIndex = 0;
        for(auto it = elemTypes.begin(); it != elemTypes.end(); it++)
        {
          if(toAddToHot.find(index) != toAddToHot.end()) // additional fields are added to the hot fields
          {
            hotFields.push_back(*it);
            hotIndices.push_back(make_pair(index,hotIndex));
            hotIndex++;
          }
          else if(elems.at(index) < mean) // create cold fields
          {
            coldFields.push_back(*it);
            coldIndices.push_back(make_pair(index,coldIndex));
            coldIndex++;
          }

          index++;
        }

        /// END OF AFFINITY GROUP CHECK

        if(coldFields.size() == 0) //no cold field required for this struct
        {
          errs()<<"No need to split this struct - no cold fields found\n";
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

        for(int j = 0; j < coldIndices.size(); j++)
        {
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

        //// create new hot and cold structs
        //// for hot struct, replace fields of current struct, keeping the same struct name, and add a new ptr field to the cold struct
        //// for cold struct, create new struct with the cold fields, with new struct name

        ArrayRef<Type*> newColdFields = ArrayRef(coldFields);
        
        /* creating cold struct */

        string coldName = currStruct->getName().str();
        coldName = coldName.append("Cold");
        StructType* coldStruct = StructType::create(split_Context, coldName);
        coldStruct->setBody(coldFields);

        coldStructs.push_back(coldStruct);

        PointerType* coldStructPtr = PointerType::get(coldStruct, 0); // Initialise the cold struct pointer
        int coldStructPtrIndex = hotIndex++;
        errs()<<"Cold struct accessed at index: "<<coldStructPtrIndex<<"\n";

        // update the AoS information in 'aosValues' to store the field index of the cold struct
        for(int s = 0; s < aosValues.size(); s++)
        {
          StructType* structure = get<1>(aosValues.at(s));
          if(structure == currStruct)
            get<2>(aosValues.at(s)) = coldStructPtrIndex;
        }

        /* creating hot struct */

        hotFields.push_back(coldStructPtr); // add the cold struct ptr to the 'hot' fields
        ArrayRef<Type*> newHotFields = ArrayRef(hotFields);

        StructType* temporaryStruct = StructType::create(split_Context, allStructs.at(i)->getName());

        temporaryStruct->setBody(newHotFields); // to calculate size of hot struct

        int originalSize = origStructSizes.find(allStructs.at(i))->second.first;

        int newSize = TD->getTypeAllocSize(temporaryStruct);

        if(newSize > originalSize) // don't bother splitting if the struct size is the same after splitting
        {
          if(newSize == originalSize) // this is done to fix errors when attempting to split dynamicAoS.ll 
            errs()<<"Hot struct same in size after splitting. Not feasible to split this structure.\n";
          else
            errs()<<"Hot struct larger in size after splitting. Not feasible to split this structure.\n";

          coldStructs.pop_back();
          origStructSizes.erase(coldStruct);

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
        
        allStructs.at(i)->setBody(newHotFields);

        currStruct->setBody(newHotFields);

        // update map to insert new hot and cold structs

        auto updateMap = origStructSizes.find(currStruct);
        if(updateMap != origStructSizes.end())  
        {
          updateMap->second.first = updateMap->second.second; // update the old size
          updateMap->second.second = newSize; // update the new size
        } 

        origStructSizes.insert(make_pair(coldStruct,make_pair(TD->getTypeAllocSize(coldStruct),TD->getTypeAllocSize(coldStruct))));

        /// print out the 'hot' and 'cold' structs

        errs()<<"Hot Struct:\n";
        currStruct->print(errs());
        errs()<<"\n";
        errs()<<"Size change: "<<originalSize<<" -> "<<TD->getTypeAllocSize(temporaryStruct)<<"\n";
        for(int j = 0; j < hotIndices.size(); j++)
        {
          errs()<<"Old index: "<<hotIndices.at(j).first<<" --> New index: "<<hotIndices.at(j).second<<"\n";
        }
        errs()<<"\nCold Struct:\n";
        coldStruct->print(errs());
        errs()<<"\n";
        errs()<<"Size: "<<TD->getTypeAllocSize(coldStruct)<<"\n";

        for(int j = 0; j < coldIndices.size(); j++)
        {
          errs()<<"Old index: "<<coldIndices.at(j).first<<" --> New index: "<<coldIndices.at(j).second<<"\n";
        }

        /* update all field accesses to the new structs */

        // When an access to a field that now belongs to the cold struct is found within a block, perform these operations (in this order):
        // 1. create GEP inst to access the cold struct ptr
        // 2. create new malloc call inst to allocate data
        // 3. create store inst to store the returned malloc memory to the GEP 
        // 4. create a load inst to the GEP as a ptr, so the pointer operand and the indices of existing GEP inst can be changed for the cold fields

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
                  prec = GEP;
                  precSet = true; //flag is set to true, indicating that a global AoS is being accessed and the next GEP instruction accesses the struct fields
                } 
                else if(GEP->getSourceElementType() == currStruct & GEP->getResultElementType() != currStruct & precSet == true) //if the GEP that is accessing a struct field is found, following prec
                {
                  string operand_str;
                  raw_string_ostream opstr(operand_str);
                  Value* index = GEP->getOperand(GEP->getNumIndices()); //get last index value being access
                  index->printAsOperand(opstr);

                  size_t space_pos = operand_str.find(" ");    
                  string type;
                  string indexStr;

                  // string manipulation to get 'type' and 'name' of the indices
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
                    GEP->setOperand(GEP->getNumIndices(),ConstantInt::get(split_Context,APInt(32,newIndex))); // change the index
                  }
                  else if(!isHotIndex & isColdIndex)  // if the GEP operates on a index that should be in a cold struct
                  {

                    vector<Value*> indexVector;
                    indexVector.push_back(ConstantInt::get(split_Context,APInt(32,0)));
                    indexVector.push_back(ConstantInt::get(split_Context,APInt(32,coldStructPtrIndex)));
                    ArrayRef<Value*> indexList = ArrayRef(indexVector);
                    coldGEP = GetElementPtrInst::CreateInBounds(currStruct, prec, indexList, "cold", &I);
                   
                    if(isMalloced == false) // create a malloc() call for the cold struct ptr
                    {
                      IRBuilder<> TmpB(&I);
                      int size = TD->getTypeAllocSize(coldStruct);
                      if(size < 8)
                        size = 8;
                      Value* structSize = ConstantInt::get(M.getContext(),APInt(64,size));
                      Type* ITy = Type::getInt32Ty(split_Context);

                      CallInst* mallocInst = TmpB.CreateCall(M.getFunction("malloc"),structSize,"",nullptr);                      

                      Instruction* storeInst = new StoreInst(mallocInst,coldGEP,&I);

                      isMalloced = true;
                    }

                    /* with a coldGEP created/existing, edit the current GEP to access the cold struct by
                      - changing the sourceElementptr to cold struct
                      - changing the ptr operand to the coldGEP
                      - changing the index to the new index
                    */

                    Instruction* loadInst = new LoadInst(PointerType::get(split_Context,0),coldGEP,"",&I);

                    GEP->setSourceElementType(coldStruct);
                    GEP->setOperand(0,loadInst);
                    GEP->setOperand(GEP->getNumIndices(),ConstantInt::get(split_Context,APInt(32,newIndex))); // change the index
                    
                  }
                  precSet = false; // set this to false so next array access GEP can be found
                  isArray = false;
                }
              }
            }
          }
        }
        }

        // Create a variable in main() to signify that struct splitting has been applied to this program/IR. 
        // This variable is checked for before struct splitting is applied, to prevent applying this optimisation twice which is not permitted.

        // Create a unique global variable with these attributes
        // Similarly a programmer can include a global variable with the same name as "permitStructSplittingFlag" to disable this struct splitting optimisation for any reason.
        GlobalVariable* permitStructSplitting = new GlobalVariable(M, Type::getInt1Ty(split_Context), true, GlobalValue::LinkageTypes::PrivateLinkage, Constant::getNullValue(Type::getInt1Ty(split_Context)), "permitStructSplittingFlag");

        /* Update allocation sizes for dynamic AoS and update free() function to de-allocate the cold struct ptr */

        //// now for every AoS used as function arguments, create a function containing a for/while loop that frees the allocated cold pointer struct (if it is not NULL)
        // to determine where to create this for/while loop, get the LAST use of the AoS, and add the for/while loop in the next block

        for(int i = 0; i < aosValues.size(); i++)
        {
          Value* currentAoS = get<0>(aosValues.at(i));
          errs()<<"AOS: "<<i+1;
          currentAoS->print(errs());
          errs()<<"\n";
          StructType* currentStruct = get<1>(aosValues.at(i));
          int coldStructPtrIndex = get<2>(aosValues.at(i));
          bool needToCreateFunc = false;
          auto findElem = functionCreated.find(currentStruct);
          if(findElem != functionCreated.end())
            needToCreateFunc = findElem->second;

          bool mallocFound = false;

          //update malloc() call for this AoS
          for(auto u = currentAoS->users().begin(); u != currentAoS->users().end(); u++)
          {
            Instruction* inst = cast<Instruction>(*u);
            if(auto *SI = dyn_cast<StoreInst>(inst))
            {
              Instruction* pred = SI->getPrevNode();
              if(auto *CI = dyn_cast<CallInst>(pred))
              {
                if(CI->getCalledFunction()->getName() == "malloc") //replace the malloc with a new one, allocating the reduced size
                {
                  mallocFound = true;

                  Instruction* next = CI->getNextNode();
                  
                  int newSize = origStructSizes.find(currentStruct)->second.second;
                  Value* structSize = ConstantInt::get(M.getContext(),APInt(64,newSize));

                  IRBuilder<> TmpB(next);

                  int origSize = origStructSizes.find(currentStruct)->second.first;
                  Value* origSizeValue = ConstantInt::get(M.getContext(),APInt(64,origSize));

                  Value* numElements = TmpB.CreateUDiv(CI->getArgOperand(0),origSizeValue,"");
                  Value* newAllocSize = TmpB.CreateMul(numElements,structSize,"",false,false);

                  CallInst* mallocInst = TmpB.CreateCall(M.getFunction("malloc"),newAllocSize,"",nullptr);
                  CI->replaceAllUsesWith(mallocInst);
                  CI->eraseFromParent();

                  //we can use this calculated array size to free the AoS later
    
                  get<3>(aosValues.at(i)) = numElements;

                }
                 else if(CI->getCalledFunction()->getName() == "calloc") //update second operand of calloc with new struct size
                      { 
                        int newSize = origStructSizes.find(allStructs.at(i))->second.second;
                        Value* structSize = ConstantInt::get(M.getContext(),APInt(64,newSize));

                        CI->setArgOperand(1,structSize);
                        get<3>(aosValues.at(i)) = CI->getOperand(0);
                      }
              }
            }
          }

          if(mallocFound == false)
          {
            currentAoS->print(errs());
            errs()<<"\nNo malloc() found for this AoS.\n";
            continue;
          }

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

            if(auto *CI = dyn_cast<CallInst>(call))
            {
              Value* operand = CI->getArgOperand(0);
              if(auto *LI = dyn_cast<LoadInst>(operand))
              {
                if(LI->getPointerOperand() == currentAoS)
                {
                  blockToAdd = CI->getParent();
                  terminator = LI->getNextNode();
                  aosLoadInst = LI;
                  freeAoSExists = true;
                }
              }
            }
          } 

          User* lastUsage = *(currentAoS->users().begin()); // last user of the AoS

          Instruction* lastInstructionUsage = cast<Instruction>(lastUsage);

          BasicBlock* currentBlock = lastInstructionUsage->getParent(); 
          currentBlock->printAsOperand(errs());
          errs()<<" in function: ";
          currentBlock->getParent()->printAsOperand(errs());
          errs()<<"\n";
          
          if(blockToAdd == nullptr)
            blockToAdd = currentBlock->getSingleSuccessor();
          if(blockToAdd == nullptr)
          {
            auto inst = currentBlock->getTerminator();
            errs()<<"\n";
            if(auto *BR = dyn_cast<BranchInst>(inst))
            {
              blockToAdd = BR->getSuccessor(BR->getNumSuccessors() - 1);
            }
          }

          errs()<<"\n";

          IRBuilder<> FreeBuilder(terminator);

          string funcName = "freeAoS";
          funcName.append(currentStruct->getName());

          if(!needToCreateFunc) 
          {
            vector<Type *> ArgTypes;

            ArgTypes.push_back(PointerType::get(M.getContext(),0)); // AoS parameter
            ArgTypes.push_back(Type::getInt64Ty(M.getContext())); // size parameter

            FunctionType *FT = FunctionType::get(Type::getVoidTy(M.getContext()), ArgTypes, false);

            Function* freeAoSFunction = Function::Create(FT,Function::ExternalLinkage,funcName,M);

            BasicBlock *entryBlock = BasicBlock::Create(split_Context, "entry", freeAoSFunction);

            //// at entry block, obtain the size of AoS
            FreeBuilder.SetInsertPoint(entryBlock);

            // loading the function argument: aos ptr

            AllocaInst* aosStore = FreeBuilder.CreateAlloca(PointerType::get(split_Context,0), nullptr, "aos"); // stores AoS pointer
            FreeBuilder.CreateStore(freeAoSFunction->getArg(0), aosStore);

            AllocaInst* sizeStore = FreeBuilder.CreateAlloca(Type::getInt64Ty(split_Context), nullptr, "size"); // stores number of AoS elements
            FreeBuilder.CreateStore(freeAoSFunction->getArg(1), sizeStore);

            AllocaInst* indexStore = FreeBuilder.CreateAlloca(Type::getInt32Ty(split_Context), nullptr, "index"); // stores while loop index
            FreeBuilder.CreateStore(ConstantInt::get(split_Context,APInt(32,0)), indexStore); // initialise index to 0

            LoadInst* aosLoad = new LoadInst(PointerType::get(M.getContext(),0),aosStore,"",entryBlock);

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

            aosLoad = FreeBuilder.CreateLoad(PointerType::get(M.getContext(),0),aosStore,"aos"); //load aos to be accessed
            indexLoad = FreeBuilder.CreateLoad(Type::getInt32Ty(split_Context),indexStore); //load index

            //extend index from i32 to i64

            Value* extendedIndex = FreeBuilder.CreateSExt(indexLoad,Type::getInt64Ty(split_Context));

            Value* elemAccess = FreeBuilder.CreateGEP(currentStruct,aosLoad,extendedIndex,"elemAccess",true);

            vector<Value*> indices;
            indices.push_back(ConstantInt::get(split_Context,APInt(32,0)));
            indices.push_back(ConstantInt::get(split_Context,APInt(32,coldStructPtrIndex)));

            Value* coldStructPtrAccess = FreeBuilder.CreateGEP(currentStruct,elemAccess,indices,"coldStructPtr",true);

            LoadInst* gepLoad = FreeBuilder.CreateLoad(coldStructPtrAccess->getType(),coldStructPtrAccess,"coldStruct");

            Value* freeColdStruct = FreeBuilder.CreateCall(M.getFunction("free"),gepLoad,"");

            indexLoad = FreeBuilder.CreateLoad(Type::getInt32Ty(split_Context),indexStore); //load index
            Value* inc = FreeBuilder.CreateAdd(indexLoad,ConstantInt::get(split_Context,APInt(32,1)),"",false,true);
            FreeBuilder.CreateStore(inc,indexStore);

            FreeBuilder.CreateBr(whileCond);

            functionCreated.find(currentStruct)->second = true;
            errs()<<"Created function: "<<funcName<<"\n";
          }

          FreeBuilder.SetInsertPoint(terminator);

          if(aosLoadInst == nullptr)
            aosLoadInst = new LoadInst(currentAoS->getType(),currentAoS,"",terminator);

          vector<Value*> arguments;
          arguments.push_back(aosLoadInst);
          arguments.push_back(get<3>(aosValues.at(i)));

          CallInst* freeAoSCall = FreeBuilder.CreateCall(M.getFunction(funcName),arguments,"",nullptr); // insert the function call after the last usage of the AoS

        }

        errs()<<"\n----------------------- END OF STRUCT SPLITTING -----------------------\n";

        return PreservedAnalyses::none();
    };
};

}