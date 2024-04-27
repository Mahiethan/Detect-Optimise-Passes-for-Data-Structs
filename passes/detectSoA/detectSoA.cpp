//CONDITION: for dynamic SoA, the array must be populated with some data, either explicitly in the main function or via a function
//empty dynamic SoA will not be detected 

#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include "../detectAoS/detectAoS.h"

#include <algorithm>
#include <string>
#include <deque>

using namespace llvm;
using namespace std;

vector<tuple<Value*,Function*,string,StructType*>> potentialSoA; // stores list of pointers that potentially be storing an SoA
vector<tuple<Value*,Function*,string,StructType*>> potentialArgumentsSoA; //stores list of arguments used in function calls that could be SoA
vector<tuple<Value*,StoreInst*,string>> argStoresSoA; // stores list of StoreInst that stores the arguments within the function body
vector<tuple<Value*,Function*,StructType*>> possibleGlobalsSoA; // stores list of global variables that potentially be an SoA
set<tuple<int,string,vector<int>,Value*,Function*,string>> calledFunctionSoA; // stores pair of function name and used argument index of pointer (if any)
int calledNumSoA = 0; // index used to store each called function uniquely in a set.

map<string,set<string>> functionStreamSoA; //for each function, stores names of functions that may be called within in
 
Function* originFunctionSoA = NULL; //temporary variable used to store original function of current SoA being inspected

int nonPointerCount = 0; // total number of found non-pointer SoA
int pointerCount = 0; // total number of found pointer SoA
bool mallocFlagSoA = false; // flag used to check for StoreInst right after a malloc() function call is found

// temporary data structures used to obtain original SoA names and functions (if they are used in function calls)
set<CallInst*> searchedCISoA;
vector<Function*> searchedFunctionCallsSoA;

// ------------------------------------------- START OF HELPER FUNCTIONS -------------------------------------------

// find origin of a function call
pair<Value*,Function*> getOriginSoA(Function* toSearch, Value* call, Module* M)
{
  pair<Value*,Function*> result = make_pair(call,toSearch);
      for (auto &F : *M)
      {
        for (auto &B : F)
        {
          for (auto &I : B)
          {
            if(auto *CI = dyn_cast<CallInst>(&I))
            {
              Function* calledFunctionSoA = CI->getCalledFunction();
              Function* parentFunction = &F;
              if(calledFunctionSoA == toSearch & searchedCISoA.find(CI) == searchedCISoA.end())
              {
                searchedCISoA.insert(CI);
                searchedFunctionCallsSoA.push_back(parentFunction);
                return make_pair(CI,parentFunction);
              }  
            }
           }
        }
      }
    return result;
  }

// obtain statically declared SoA
void detectNonPointerSoA(AllocaInst* AI)
{
  Type* returnType = AI->getAllocatedType();

  //get type as a string
  std::string type_str; 
  raw_string_ostream to_str(type_str);
  returnType->print(to_str);

  if(!AI->isArrayAllocation()) //identifying SoA with size given as a variable - if the alloca instruction has a size attribute that is not 1 (i.e. not a single allocation of the type)
  {
      if(toFind.find(returnType) != toFind.end()) //if the allocated type is a struct
      {
        Value* SoA = cast<Instruction>(AI); //store as Value in vector
        StructType* structure = cast<StructType>(returnType);
        potentialSoA.push_back(make_tuple(SoA,originFunctionSoA,"static",structure));
      }
  }
  else //AoSoA
  {
    if(toFind.find(returnType) != toFind.end()) //if the allocated type is a struct
      {
        Value* SoA = cast<Instruction>(AI); //store as Value in vector
        StructType* structure = cast<StructType>(returnType);
        AoSoAList.push_back(make_tuple(SoA,originFunctionSoA,"static",structure));
      }
  }
}

// check whether an SoA is found in the 'confirmed' list
bool findInConfirmedSoA(Value* val)
{
  for(auto it = confirmedSoA.begin(); it != confirmedSoA.end(); it)
  {
    Value* soa = get<0>(*it);
  
    if(soa == val)
    {
      printf("Found in confirmedSoA\n");
      return true;
    }
    else
      it++;
  }
  return false;
}

// erase an SoA from the 'confirmed' list, if found.
StructType* eraseFromConfirmedSoA(Value* val)
{
  for(auto it = confirmedSoA.begin(); it != confirmedSoA.end(); it)
  {
    Value* soa = get<0>(*it);
    string type = get<2>(*it);
    StructType* structure = get<3>(*it);
    std::string soa_string; 
    raw_string_ostream soa_stream(soa_string);
    soa->printAsOperand(soa_stream);
    if(soa == val)
    {
      confirmedSoA.erase(it);
    if(type == "static")
          nonPointerCount--;
    else if(type == "dynamic")
      pointerCount--;

      return structure;
    }
    else
    {
    it++;
    }
  }
  return nullptr;
}

// erase the global SoA from the 'possibleGlobals' list if found
StructType* eraseFromPossibleGlobalsSoA(Value* val)
{
  for(auto it = possibleGlobalsSoA.begin(); it != possibleGlobalsSoA.end(); it)
  {
    Value* soa = get<0>(*it);
    StructType* structure = get<2>(*it);

    if(soa == val)
    {
      possibleGlobalsSoA.erase(it);
      return structure;
    }
    else
    {
    it++;
    }
  }
  return nullptr;
}

// to check whether the next function call will eventually led to the current function, making it recursive
bool checkIfRecursiveSoA(string nextFunction, string currentFunction)
{
  if(functionStreamSoA.find(nextFunction) != functionStreamSoA.end())
  {
    set<string> nextList = functionStreamSoA.find(nextFunction)->second;
    if(nextList.find(currentFunction) != nextList.end())
      return true;
  }
  return false;
}

//function used to add an SoA to potential vector
void getPotentialSoA(Instruction* I) //adding to potentialSoA vector
{
  if(auto *SI = dyn_cast<StoreInst>(I))
  {
    Value* operand = SI->getOperand(1); //get second operand of this store instruction
    
    std::string soa_string; 
    raw_string_ostream soa_stream(soa_string);
    operand->printAsOperand(soa_stream);

    bool foundInPossibleGlobals = false;
    bool foundInPotential = false;
    Value* soa;
    Function* func;
    for(auto it = possibleGlobalsSoA.begin(); it != possibleGlobalsSoA.end(); it)
    {
      soa = get<0>(*it);
      func = get<1>(*it);

      if(soa == operand)
      {
        foundInPossibleGlobals = true;
        break;
      }
      else
      {
      it++;
      }
    }
    if(foundInPossibleGlobals)
    {
      StructType* structure = eraseFromPossibleGlobalsSoA(operand);
      if(!findInConfirmedSoA(operand))
      {
        confirmedSoA.push_back(make_tuple(soa,func,"dynamic",structure,false,false));
        pointerCount++;
      }
    }
    else
    {
      for(auto it = potentialSoA.begin(); it != potentialSoA.end(); it)
      {
        soa = get<0>(*it);
        func = get<1>(*it);

        if(soa == operand & func == originFunctionSoA)
        {
          foundInPotential = true;
          break;
        }
        else
        {
        it++;
        }
      }

      if(!foundInPotential)
      {
        StructType* structure = eraseFromConfirmedSoA(operand); //remove from confirmedSoA list if it exists before adding it to potentialSoA
        potentialSoA.push_back(make_tuple(operand,originFunctionSoA,"dynamic",structure));
      }
    }
  }  
  mallocFlagSoA = false; //disable flag - don't search for StoreInst after malloc
}

// erase an SoA from the 'potential' list if found
tuple<llvm::Value *, llvm::Function *, std::string, llvm::StructType *> eraseFromPotentialSoA(Value* val)
{
  for(auto it = potentialSoA.begin(); it != potentialSoA.end(); it)
  {
    Value* soa = get<0>(*it);
    StructType* structure = get<3>(*it);
    tuple<Value*,Function*,string,StructType*> elem = *it;

    if(soa == val)
    {
      potentialSoA.erase(it);
      return elem;
    }
    else
    {
    it++;
    }
  }
  return make_tuple(nullptr,nullptr,"",nullptr);
}

// erase an SoA function argument from the 'potentialArguments' list
void eraseFromPotentialArgumentsSoA(Value* val)
{
  for(auto it = potentialArgumentsSoA.begin(); it != potentialArgumentsSoA.end(); it)
  {
    Value* soa = get<0>(*it);
    StructType* structure = get<3>(*it);
    auto elem = *it;

    if(soa == val)
    {
      potentialArgumentsSoA.erase(it);
    }
    else
    {
    it++;
    }
  }
}

// erase an StoreInst from 'argStores' if found
pair<StoreInst*,string> eraseFromArgStoresSoA(Value* val)
{
  for(auto it = argStoresSoA.begin(); it != argStoresSoA.end(); it)
  {
    Value* soa = get<0>(*it);
    StoreInst* store = get<1>(*it);
    string type = get<2>(*it);
    std::string soa_string; 
    raw_string_ostream soa_stream(soa_string);
    soa->printAsOperand(soa_stream);

    std::string val_string; 
    raw_string_ostream val_stream(val_string);
    val->printAsOperand(val_stream);

    if(soa == val)
    {
      argStoresSoA.erase(it);
      return make_pair(store,type);
    }
    else
    {
    it++;
    }
  }
  return make_pair(nullptr,"");
}

// temporary variables to store information about current GEPInst that is being inspected
Value* gepOperandSoA = NULL;
Value* calledAoSSoA = NULL;
Function* paramFunctionSoA = NULL;

bool findInAoSoAList(Value* op)
{
  for(int i = 0; i < AoSoAList.size(); i++)
  {
    Value* toFind = get<0>(AoSoAList.at(i));
    if(toFind == op)
      return true;
  }
  return false;
}

// check whether a GEP instruction is accessing a structure element from an array
bool checkGEPSoA(GetElementPtrInst *gep, Value* soa, bool isParam, string type)
{
    Value* operand = gep->getOperand(0);
    StructType* gepStruct = nullptr;

    //getting operand as string
    std::string op_string; 
    raw_string_ostream ops(op_string);
    operand->print(ops);

    if(auto *ST = dyn_cast<StructType>(gep->getSourceElementType()))
    {
        gepStruct = ST;
    }

    while(isa<LoadInst>(operand) == false & isa<AllocaInst>(operand) == false & isa<GlobalVariable>(operand) == false)
    {
      if(auto *GEP = dyn_cast<GetElementPtrInst>(operand))
      {
        operand = GEP->getOperand(0);
      }
      else
      {
        return false;
      }
    }
    
    if(auto *GV = dyn_cast<GlobalVariable>(operand)) //GEP operating on a global SoA
    {
      std::string ptr_string; 
      raw_string_ostream ptr(ptr_string);
      soa->print(ptr);

      if(op_string == ptr_string)
      {
        if(type == "static")
          nonPointerCount++;
        else if(type == "dynamic")
          pointerCount++;

        // output not needed
        StructType* structure = get<3>(eraseFromPotentialSoA(soa));
        if(structure == nullptr)
          structure = eraseFromPossibleGlobalsSoA(soa);
        confirmedSoA.push_back(make_tuple(soa,originFunctionSoA,type,gepStruct,false,false));
        return true;
      }
    }
    else if(auto *AI = dyn_cast<AllocaInst>(operand))
    {
      std::string ptr_string; 
      raw_string_ostream ptr(ptr_string);
      soa->print(ptr);

      if(op_string == ptr_string) //if loaded operand is one of the malloc-ed arrays detected earlier
      {
        if(type == "static")
          nonPointerCount++;
        else if(type == "dynamic")
          pointerCount++;

        StructType* structure =  get<3>(eraseFromPotentialSoA(soa));
        if(structure == nullptr)
          structure = eraseFromPossibleGlobalsSoA(soa);

        if(isParam)
        {
          Value* storedAoS = eraseFromArgStoresSoA(soa).first;
          confirmedSoA.push_back(make_tuple(storedAoS,originFunctionSoA,type,gepStruct,true,false));
        }
        else
          confirmedSoA.push_back(make_tuple(soa,originFunctionSoA,type,gepStruct,false,false));

        return true;
      }
      gepOperandSoA = operand;
    }
    else if(auto *OI = dyn_cast<LoadInst>(operand)) //check if instruction is a Load instruction
    {
      Value* operand = OI->getOperand(0); //get operand that is being loaded in

      std::string op_string; 
      raw_string_ostream ops(op_string);
      operand->print(ops);
      
      std::string ptr_string; 
      raw_string_ostream ptr(ptr_string);
      soa->print(ptr);

      if(op_string == ptr_string) //if loaded operand is one of the malloc-ed arrays detected earlier
      {
       if(type == "static")
          nonPointerCount++;
        else if(type == "dynamic")
          pointerCount++;

        StructType* structure =  get<3>(eraseFromPotentialSoA(soa));
        if(structure == nullptr)
          structure = eraseFromPossibleGlobalsSoA(soa);

        if(isParam)
        {
          Value* storedAoS = eraseFromArgStoresSoA(soa).first;
          confirmedSoA.push_back(make_tuple(storedAoS,originFunctionSoA,type,gepStruct,true,false));
        }
        else
          confirmedSoA.push_back(make_tuple(soa,originFunctionSoA,type,gepStruct,false,false));
      
        return true;
      }
      gepOperandSoA = operand;

    }
  
  return false;
}

// obtain the function that has been called using the CallInst
void getCalledFunctionsSoA(CallInst* CI, Function* orig)
{
  vector<int> indices;
  Value* soa;
  Value* paramAoS;
  string type = "undefined";
  int index = 0;

  CI->setTailCall(false);

  Function* f = CI->getCalledFunction();
  string funcName = f->getName().str();

  if(funcName == orig->getName() | checkIfRecursiveSoA(funcName,orig->getName().str())) //function call may be recursive
  {
    return;
  }
  for(auto a = CI->arg_begin(); a != CI->arg_end(); a++) //get all function arguments
  {
    bool detect = false;
    if(detect == true)
    {
      break;
    }

    Value* argument = a->get(); //get argument operand
    Value* operand;
    if(auto LI = dyn_cast<LoadInst>(argument))
      operand = LI->getOperand(0); //get first operand (value being loaded)      
    else
      operand = argument;

    //convert to string for comparison
    std::string op_string; 
    raw_string_ostream ops(op_string);
    operand->printAsOperand(ops);
      
      for(int i = 0; i < potentialSoA.size(); i++)
      {
        soa = get<0>(potentialSoA.at(i));
        std::string value_string; 
        raw_string_ostream aos_stream(value_string);
        soa->printAsOperand(aos_stream);

        if(value_string == op_string)
        {
          indices.push_back(index);
          //add to new list: potentialArgumentsSoA
          tuple<Value*,Function*,string,StructType*> elem = eraseFromPotentialSoA(soa);
          type = get<2>(elem);
          potentialArgumentsSoA.push_back(elem);
          detect = true;
          paramAoS = soa;

          break;
        }
      }
      if(detect == false)
      {
        for(int i = 0; i < argStoresSoA.size(); i++)
        {
          soa = get<0>(argStoresSoA.at(i));
          std::string value_string; 
          raw_string_ostream aos_stream(value_string);
          soa->printAsOperand(aos_stream);

          if(value_string == op_string)
          {
            detect = true;
            indices.push_back(index);
            eraseFromArgStoresSoA(soa);
            paramAoS = soa;

            break;
          }
        }
      }
      if(detect == false) //add to possible globals
      {
        if(auto *GV = dyn_cast<GlobalVariable>(operand)) //if a function call has a global parameter not yet malloced, add it to possibleGlobalsSoA
        {
          bool inConfirmed = findInConfirmedSoA(operand);
          if(inConfirmed == false & (findInAoSoAList(operand) == false))
            possibleGlobalsSoA.push_back(make_tuple(operand,f,nullptr)); 
          detect = true;
        }
      }
    
    index++;
  }

  calledFunctionSoA.insert(std::make_tuple(calledNumSoA,funcName,indices,paramAoS,orig,type));
  calledNumSoA++;
}

// obtain the StoreInst that stores the potential SoA argument 
vector<tuple<Value*,StoreInst*,string>> getArgumentStoresSoA(Function *F, vector<int> indices,string type)
{
  vector<Value*> arguments;
  vector<tuple<Value*,StoreInst*,string>> newStores;
  for(int i = 0; i < indices.size(); i++)
  {
    arguments.push_back(F->getArg(indices.at(i)));
  }
  for (auto &B : *F)  //iterate through all blocks in each function
  { 
      for (auto &I : B) //iterate through all instructions in each block
      { 
         if(auto *SI = dyn_cast<StoreInst>(&I))
         {
          Value* operand = SI->getOperand(0); //get first operand of this store instruction

          //getting operand as string
          std::string op_string; 
          raw_string_ostream ops(op_string);
          operand->printAsOperand(ops);
          
          for(int i = 0; i < arguments.size(); i++)
          {
            std::string arg_string; 
            raw_string_ostream args(arg_string);
            arguments.at(i)->printAsOperand(args);
            if(arg_string == op_string)
            {
              Value* arrayStore = SI->getOperand(1); //get second operand of this store instruction
              newStores.push_back(make_tuple(arrayStore,SI,type));
              break;
            }
          }

          if(newStores.size() == indices.size())
            break;
        }
      }
  }
  return newStores;
}

// ------------------------------------------- END OF HELPER FUNCTIONS -------------------------------------------

// ------------------------------------------- START OF SOA DETECTION CODE -------------------------------------------

namespace {

struct detectSoA : public PassInfoMixin<detectSoA> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {

        errs()<<"\n-------------------------- SOA DETECTION --------------------------\n\n";

        detectAoSCalled = true;

        vector<Value*> possibleAoSList; //vector which stores pointer locations of possible SoA - these need to be inspected and confirmedSoA
        vector<Value*> AoS_Values; //vector which stores pointer locations of possible SoA - these need to be inspected and confirmedSoA
        deque<tuple<string, int, Value*>> functionInspectionList; //vector that stores functions that need to be inspected for GPE instructions in order to fully determine that a dynamic SoA exists

        vector<StructType*> allStructs = M.getIdentifiedStructTypes(); //get all struct types used in program

        DataLayout* TD = new DataLayout(&M);

        //for each struct, look through all element tyes

        //if all elements are of aggregrate types, with size > 1, then it can be used as an SoA
        // add this struct to "toFind" vector

        for(int i = 0; i < allStructs.size(); i++)
        {
          StructType* currStruct = allStructs.at(i);

          // no. of fields in struct
          int numFields = currStruct->getStructNumElements();

          // boolean to determine whether all fields of the struct are arrays
          // set to true initially, will be set to false if one field is not an array, of size > 1
          bool allFieldsArrays = true;

          for(int i = 0; i < numFields; i++)
          {
            // get type of field
            Type* fieldType = currStruct->getElementType(i);

            if(fieldType->isArrayTy())
            {
              int arrayElements = fieldType->getArrayNumElements();
              if(arrayElements > 1)
                allFieldsArrays = true;
              else
                allFieldsArrays = false;
            }
            else
            {
              // if a field is not an array, it is not a SoA
              allFieldsArrays = false;
            }

            if(allFieldsArrays == false)
            {
              currStruct->print(errs());
              errs()<<" is not a SoA\n\n";
              break;
            }
          }

          if(allFieldsArrays == true)
          {
            currStruct->print(errs());
            errs()<<" added to SoA toFind list";

            int size = TD->getTypeAllocSize(currStruct);

            errs()<<" with size "<<size<<" bytes.\n\n";

            toFind.insert(make_pair(currStruct,size));
          }

        }

        for(auto Global = M.global_begin(); Global != M.global_end(); ++Global)
        {
          Constant* constValue = nullptr; 
          if(Global->hasInitializer())
            constValue = Global->getInitializer(); //get the initializer
          else
            continue;

          Type* t = constValue->getType(); //get the type of the global
          string type_str;
          raw_string_ostream tstr(type_str);
          t->print(tstr);

          if(auto *AR = dyn_cast<ArrayType>(t))
          {
            t = AR->getArrayElementType();

            if(toFind.find(t) != toFind.end())
            {
              Value* AoSoA = cast<Value>(&(*Global)); //store as Value in vector

              StructType* structure = cast<StructType>(t);
              AoSoAList.push_back(make_tuple(AoSoA,nullptr,"static",structure));
            }
          }
          else
          {
            if(toFind.find(t) != toFind.end())
            {
              Value* SoA = cast<Value>(&(*Global)); //store as Value in vector
              Type* structure = toFind.find(t)->first;
              StructType* ST = cast<StructType>(structure);

              potentialSoA.push_back(make_tuple(SoA,originFunctionSoA,"static",ST));
            }
          }
        }
        
        bool checkMain = false;
        for (auto &F : M) //iterate through all functions in the Module and print their names
        { 
            if(checkMain == true)
              break;

            if(F.getName().find("main") != std::string::npos)
            {
              originFunctionSoA = &F;
              for (auto &B : F)  //iterate through all blocks in each function
              { 
                  for (auto &I : B) //iterate through all instructions in each block
                  { 
                    if(mallocFlagSoA == true)
                      getPotentialSoA(&I);
                    else if(auto *SI = dyn_cast<StoreInst>(&I)) //find global static SoA used in a GEP operation
                    {
                      bool found = false;
                      Value* operand = SI->getOperand(1);
                      std::string op_str; 
                      raw_string_ostream to_str(op_str);
                      operand->printAsOperand(to_str);

                      Value* loadedAoS = SI->getOperand(0);

                      if(auto *LI = dyn_cast<LoadInst>(SI->getOperand(0)))
                      {
                        loadedAoS = LI->getOperand(0);
                      }

                      for(auto it = potentialSoA.begin(); it != potentialSoA.end(); it++)
                      {
                        Value* soa = get<0>(*it);
                        std::string ptr_string; 
                        raw_string_ostream ptr(ptr_string);
                        soa->printAsOperand(ptr);

                        if((op_str.find("getelementptr") != std::string::npos) & (op_str.find(ptr_string) != std::string::npos))
                        {
                          StructType* structure =  get<3>(eraseFromPotentialSoA(soa));
                          nonPointerCount++;
                          confirmedSoA.push_back(make_tuple(soa,originFunctionSoA,"static",structure,false,false));
                          found = true;
                          break;
                        }
                      }

                      if(found == false)
                      {
                        for(auto it = potentialSoA.begin(); it != potentialSoA.end(); it++)
                        {
                          Value* soa = get<0>(*it);
                          string type = get<2>(*it);
                          StructType* structure = get<3>(*it);

                          if(loadedAoS == soa) //an potentialSoA SoA could be stored into an empty pointer - add this pointer to 'potentialSoA' list
                          {
                            eraseFromPossibleGlobalsSoA(SI->getOperand(1));
                            potentialSoA.push_back(make_tuple(SI->getOperand(1),originFunctionSoA,type,structure));
                            found = true;
                            break;
                          }
                        }

                        for(auto it = confirmedSoA.begin(); it != confirmedSoA.end(); it++)
                        {
                          Value* soa = get<0>(*it);
                          string type = get<2>(*it);
                          StructType* structure = get<3>(*it);

                          if(loadedAoS == soa) //a potential SoA could be stored into an empty pointer - add this pointer to 'potential' list
                          {
                            eraseFromPossibleGlobalsSoA(SI->getOperand(1));
                            confirmedSoA.push_back(make_tuple(SI->getOperand(1),originFunctionSoA,type,structure,false,false));
                            found = true;
                            break;
                          }
                        }
                      }
                    }
                    else if(auto *AI = dyn_cast<AllocaInst>(&I))
                      detectNonPointerSoA(AI);
                    else if(auto *CI = dyn_cast<CallInst>(&I))
                    {
                      CI->setTailCall(false);

                      Function* f = CI->getCalledFunction();
                      string funcName = f->getName().str();

                      if(funcName == "malloc" | funcName == "calloc")
                      {
                          bool findSoA = false, findAoSoA = false;
                          if(f->getName() == "malloc")
                          {
                            Value* size = CI->getOperand(0); //only one function argument for malloc()

                            if(auto *CO = dyn_cast<ConstantInt>(size))
                            {
                              int constIntValue = CO->getSExtValue();

                              Type* foundStruct = nullptr;

                              for(auto it = toFind.begin(); it != toFind.end(); it++)
                              {
                                errs()<<it->second<<" divide by "<<constIntValue<<"\n";
                                if(it->second == constIntValue) //if size is exactly equal to a struct in toFind, this malloc-ed ptr could potentially be an SoA
                                {
                                  findSoA = true;
                                  foundStruct = it->first;
                                  break;
                                }
                                else if((constIntValue % it->second == 0) & (it->second != constIntValue)) //if size is a multiple of an SoA size, it could be an AoSoA
                                {
                                  findAoSoA = true;
                                  foundStruct = it->first;
                                  break;
                                }
                              }

                              if(findSoA)
                              {
                                //the next instruction after the malloc or calloc should be a store to the SoA
                                mallocFlagSoA = true;
                              }
                              else if(findAoSoA)
                              {
                                //the next instruction after the malloc or calloc should be a store to the SoA
                                if(auto *SI = dyn_cast<StoreInst>(I.getNextNode()))
                                {
                                  Value* AoSoA = SI->getPointerOperand(); //get the SoA
                                  StructType* structure = cast<StructType>(foundStruct);
                                  AoSoAList.push_back(make_tuple(AoSoA,&F,"dynamic",structure));
                                }
                              }
                            }
                            else //assume it is an AoSoA if the size cannot be determined (not a constant int)
                            {
                              if(auto *SI = dyn_cast<StoreInst>(I.getNextNode()))
                              {
                                Value* AoSoA = SI->getPointerOperand(); //get the SoA
                                AoSoAList.push_back(make_tuple(AoSoA,&F,"dynamic",nullptr));
                              }
                            }
                          }
                          else if(f->getName() == "calloc")
                          {
                            Value* size = CI->getOperand(1); //second function argument in calloc() defines size
                            Value* num = CI->getOperand(0); //first function argument in calloc() defines number of elements
                            // if num > 1, AoSoA found, else if num == 1, SoA found

                            if(auto *CO = dyn_cast<ConstantInt>(num))
                            {
                              int numIntValue = CO->getSExtValue();
                              if(numIntValue > 1)
                              {
                                if(auto *SI = dyn_cast<StoreInst>(I.getNextNode()))
                                {
                                  Value* AoSoA = SI->getPointerOperand(); //get the SoA
                                  AoSoAList.push_back(make_tuple(AoSoA,&F,"dynamic",nullptr));
                                  continue;
                                }
                              }
                            }
                            else //size is not a constant and cannot be determined so assume this is an AoSoA
                            {
                              if(auto *SI = dyn_cast<StoreInst>(I.getNextNode()))
                              {
                                Value* AoSoA = SI->getPointerOperand(); //get the SoA
                                AoSoAList.push_back(make_tuple(AoSoA,&F,"dynamic",nullptr));
                                continue;
                              }
                            }

                            if(auto *CO = dyn_cast<ConstantInt>(size))
                            {
                              int constIntValue = CO->getSExtValue();

                              Type* foundStruct = nullptr;

                              for(auto it = toFind.begin(); it != toFind.end(); it++)
                              {
                                if(it->second == constIntValue) //if size is exactly equal to a struct in toFind, this malloc-ed ptr could potentially be an SoA
                                {
                                  findSoA = true;
                                  break;
                                }
                                else if((constIntValue % it->second == 0) & (it->second != constIntValue)) //if size is a multiple of an SoA size, it could be an AoSoA
                                {
                                  findAoSoA = true;
                                  break;
                                }
                              }

                              if(findSoA)
                              {
                                //the next instruction after the malloc or calloc should be a store to the SoA
                                mallocFlagSoA = true;
                              }
                              else if(findAoSoA)
                              {
                                //the next instruction after the malloc or calloc should be a store to the SoA
                                if(auto *SI = dyn_cast<StoreInst>(I.getNextNode()))
                                {
                                  Value* AoSoA = SI->getPointerOperand(); //get the SoA
                                  StructType* structure = cast<StructType>(foundStruct);
                                  AoSoAList.push_back(make_tuple(AoSoA,&F,"dynamic",structure));
                                }
                              }
                            }
                            else //assume it is an AoSoA if the size cannot be determines (not a constant int)
                            {
                              if(auto *SI = dyn_cast<StoreInst>(I.getNextNode()))
                              {
                                Value* AoSoA = SI->getPointerOperand(); //get the SoA
                                AoSoAList.push_back(make_tuple(AoSoA,&F,"dynamic",nullptr));
                              }
                            }
                          }
                      }
                      else if(funcName != "free")
                      {
                        if(auto *SI = dyn_cast<StoreInst>(I.getNextNode()))
                        {
                          if(f->getReturnType()->isPointerTy() & (SI->getValueOperand() == CI))
                          {
                            potentialSoA.push_back(make_tuple(SI->getPointerOperand(),originFunctionSoA,"dynamic",nullptr));
                          }
                        }
                        getCalledFunctionsSoA(CI,&F);
                      }
                    }
                    else if(auto *GEP = dyn_cast<GetElementPtrInst>(&I))
                    {
                      bool found = false;
                      Value* soa;
                      string type;
                      if((toFind.find(GEP->getSourceElementType()) != toFind.end()))
                      {
                        for(auto it = potentialSoA.begin(); it != potentialSoA.end(); it++)
                        {
                          soa = get<0>(*it);
                          type = get<2>(*it);
                          std::string ptr_string; 
                          raw_string_ostream ptr(ptr_string);
                          soa->printAsOperand(ptr);

                          if(checkGEPSoA(GEP,soa,false,type) == true)
                          {
                            found = true;
                            eraseFromPotentialSoA(soa);
                            break;
                          }
                        }

                        if(found == false)
                        {
                          if(gepOperandSoA != NULL)
                          {
                            if(auto *GV = dyn_cast<GlobalVariable>(gepOperandSoA))
                            {
                              StructType* structure = cast<StructType>(GEP->getSourceElementType());
                              bool inConfirmed = findInConfirmedSoA(gepOperandSoA);
                              if(inConfirmed == false & (findInAoSoAList(gepOperandSoA) == false))
                                possibleGlobalsSoA.push_back(make_tuple(gepOperandSoA,originFunctionSoA,structure)); 
                            }
                          }
                        }
                      }
                    }
                  }
              checkMain = true;
              }
            }
        }

      // Now go through all called functions and search for any newly declared SoA values or search for any 'potential' SoA values that are being populated
      for(auto it = calledFunctionSoA.begin(); it != calledFunctionSoA.end(); it++)
      {
        tuple<int,string,vector<int>,Value*,Function*,string> searchFunc = *it;
        string type = get<5>(searchFunc);
        vector<int> argIndices = get<2>(searchFunc);
        calledAoSSoA = get<3>(searchFunc); //get SoA that was used as parameter in function call
        paramFunctionSoA = get<4>(searchFunc);
        string currentFuncName = get<1>(searchFunc);

        for (auto &F : M) //iterate through all functions in the Module and print their names
        { 
          if(F.getName() == get<1>(searchFunc))
          {
            set<string> next;
            functionStreamSoA.insert(make_pair(F.getName().str(),next));

            originFunctionSoA = &F;

            if(argIndices.size() != 0)
            {
              argStoresSoA = getArgumentStoresSoA(&F,argIndices,type);
            }

            for(auto &B : F)
            {
              for(auto &I : B)
              {
                if(mallocFlagSoA == true)
                  getPotentialSoA(&I);
                else if(auto *SI = dyn_cast<StoreInst>(&I)) //find global static SoA used in a GEP operation
                {
                  Value* operand = SI->getOperand(1);
                  std::string op_str; 
                  raw_string_ostream to_str(op_str);
                  operand->printAsOperand(to_str);

                  Value* loadedAoS = SI->getValueOperand();
                  if(auto *LI = dyn_cast<LoadInst>(loadedAoS))
                  {
                    loadedAoS = LI->getPointerOperand();
                  }

                  bool found = false;
                
                  for(auto it = potentialSoA.begin(); it != potentialSoA.end(); it++)
                  {
                    Value* soa = get<0>(*it);
                    std::string ptr_string; 
                    raw_string_ostream ptr(ptr_string);
                    soa->printAsOperand(ptr);

                    if((op_str.find("getelementptr") != std::string::npos) & (op_str.find(ptr_string) != std::string::npos))
                    {
                      StructType* structure =  get<3>(eraseFromPotentialSoA(soa));
                      nonPointerCount++;
                      confirmedSoA.push_back(make_tuple(soa,originFunctionSoA,"static",structure,false,false));
                      found = true;
                      break;
                    }
                  }

                  if(found == false)
                  {
                    for(auto it = potentialSoA.begin(); it != potentialSoA.end(); it++)
                    {
                      Value* soa = get<0>(*it);
                      string type = get<2>(*it);
                      StructType* structure = get<3>(*it);

                      if(loadedAoS == soa) //a potential SoA could be stored into an empty pointer - add this pointer to 'potential' list
                      {
                        eraseFromPossibleGlobalsSoA(SI->getOperand(1));
                        potentialSoA.push_back(make_tuple(SI->getOperand(1),originFunctionSoA,type,structure));
                        found = true;
                        break;
                      }
                    }

                    for(auto it = confirmedSoA.begin(); it != confirmedSoA.end(); it++)
                    {
                      Value* soa = get<0>(*it);
                      string type = get<2>(*it);
                      StructType* structure = get<3>(*it);

                      if(loadedAoS == soa) //a potential SoA could be stored into an empty pointer - add this pointer to 'potential' list
                      {
                        eraseFromPossibleGlobalsSoA(SI->getOperand(1));
                        confirmedSoA.push_back(make_tuple(SI->getOperand(1),originFunctionSoA,type,structure,false,false));
                        found = true;
                        break;
                      }
                    }
                  }

                }
                else if(auto *AI = dyn_cast<AllocaInst>(&I))
                  detectNonPointerSoA(AI);
                else if(auto *CI = dyn_cast<CallInst>(&I))
                {

                  CI->setTailCall(false);

                  Function* f = CI->getCalledFunction();
                  string funcName = f->getName().str();

                  if(funcName == "malloc" | funcName == "calloc")
                  {
                    bool findSoA = false, findAoSoA = false;
                    if(f->getName() == "malloc")
                    {
                      Value* size = CI->getOperand(0); //only one function argument for malloc()

                      if(auto *CO = dyn_cast<ConstantInt>(size))
                      {
                        int constIntValue = CO->getSExtValue();
                        // errs()<<"Size :"<<constIntValue<<"\n";

                        // StructType* SoA = nullptr;

                        Type* foundStruct = nullptr;

                        for(auto it = toFind.begin(); it != toFind.end(); it++)
                        {
                          if(it->second == constIntValue) //if size is exactly equal to a struct in toFind, this malloc-ed ptr could potentially be an SoA
                          {
                            findSoA = true;
                            foundStruct = it->first;
                            break;
                          }
                          else if((constIntValue % it->second == 0) & (it->second != constIntValue)) //if size is a multiple of an SoA size, it could be an AoSoA
                          {
                            findAoSoA = true;
                            foundStruct = it->first;
                            break;
                          }
                        }

                        if(findSoA)
                        {
                          //the next instruction after the malloc or calloc should be a store to the SoA
                          mallocFlagSoA = true;
                        }
                        else if(findAoSoA)
                        {
                          //the next instruction after the malloc or calloc should be a store to the SoA
                          if(auto *SI = dyn_cast<StoreInst>(I.getNextNode()))
                          {
                            Value* AoSoA = SI->getPointerOperand(); //get the SoA
                            StructType* structure = cast<StructType>(foundStruct);
                            AoSoAList.push_back(make_tuple(AoSoA,&F,"dynamic",structure));
                          }
                        }
                      }
                      else //assume it is an AoSoA if the size cannot be determines (not a constant int)
                      {
                        if(auto *SI = dyn_cast<StoreInst>(I.getNextNode()))
                        {
                          Value* AoSoA = SI->getPointerOperand(); //get the SoA
                          AoSoAList.push_back(make_tuple(AoSoA,&F,"dynamic",nullptr));
                        }
                      }
                    }
                    else if(f->getName() == "calloc")
                    {
                      Value* size = CI->getOperand(1); //first function argument in calloc() defines size
                      Value* num = CI->getOperand(0); //second function argument in calloc() defines number of elements
                      // if num > 1, AoSoA found, else if num == 1, SoA found

                      if(auto *CO = dyn_cast<ConstantInt>(num))
                      {
                        int numIntValue = CO->getSExtValue();
                        if(numIntValue > 1)
                          continue; //potetial AoSoA found, skip
                      }

                      if(auto *CO = dyn_cast<ConstantInt>(size))
                      {
                        int constIntValue = CO->getSExtValue();

                        Type* foundStruct = nullptr;

                        for(auto it = toFind.begin(); it != toFind.end(); it++)
                        {
                          if(it->second == constIntValue) //if size is exactly equal to a struct in toFind, this malloc-ed ptr could potentially be an SoA
                          {
                            findSoA = true;
                            break;
                          }
                          else if((constIntValue % it->second == 0) & (it->second != constIntValue)) //if size is a multiple of an SoA size, it could be an AoSoA
                          {
                            findAoSoA = true;
                            break;
                          }
                        }

                        if(findSoA)
                        {
                          //the next instruction after the malloc or calloc should be a store to the SoA
                          mallocFlagSoA = true;
                        }
                        else if(findAoSoA)
                        {
                          //the next instruction after the malloc or calloc should be a store to the SoA
                          if(auto *SI = dyn_cast<StoreInst>(I.getNextNode()))
                          {
                            Value* AoSoA = SI->getPointerOperand(); //get the SoA
                            StructType* structure = cast<StructType>(foundStruct);
                            AoSoAList.push_back(make_tuple(AoSoA,&F,"dynamic",structure));
                          }
                        }
                      }
                      else //assume it is an AoSoA if the size cannot be determines (not a constant int)
                      {
                        if(auto *SI = dyn_cast<StoreInst>(I.getNextNode()))
                        {
                          Value* AoSoA = SI->getPointerOperand(); //get the SoA
                          AoSoAList.push_back(make_tuple(AoSoA,&F,"dynamic",nullptr));
                        }
                      }
                    }
                  // mallocAttributes = f->getAttributes();
                  }
                  else if(funcName != "free")
                  {
                    if(auto *SI = dyn_cast<StoreInst>(I.getNextNode()))
                    {
                      if(f->getReturnType()->isPointerTy() & (SI->getValueOperand() == CI))
                      {
                        potentialSoA.push_back(make_tuple(SI->getPointerOperand(),originFunctionSoA,"dynamic",nullptr));
                      }
                    }

                    if(functionStreamSoA.find(funcName) != functionStreamSoA.end()) //store all of the next functions
                    {
                      if(functionStreamSoA.find(currentFuncName) != functionStreamSoA.end()) //store all of the next functions
                      {
                        functionStreamSoA.find(currentFuncName)->second.insert(funcName);
                        set<string> toAdd = functionStreamSoA.find(funcName)->second;
                        for(auto it : toAdd)
                        {
                          functionStreamSoA.find(currentFuncName)->second.insert(it);
                        }
                      }
                    }

                    getCalledFunctionsSoA(CI,&F);
                  }
                }
                else if(auto *GEP = dyn_cast<GetElementPtrInst>(&I))
                {
                  if((toFind.find(GEP->getSourceElementType()) != toFind.end()))
                  {
                    bool found = false;
                    Value* soa;
                    string type;
                    for(auto it = potentialSoA.begin(); it != potentialSoA.end(); it++)
                    {
                      soa = get<0>(*it);
                      type = get<2>(*it);
                      if(checkGEPSoA(GEP,soa,false,type) == true)
                      {
                        found = true;
                        eraseFromPotentialSoA(soa);
                        break;
                      }
                    }
                    for(auto it = argStoresSoA.begin(); it != argStoresSoA.end(); it++)
                    {
                      soa = get<0>(*it);
                      type = get<2>(*it);
                      if(checkGEPSoA(GEP,soa,true,type) == true)
                      {
                        found = true;
                        break;
                      }
                    }
                    if(found == false)
                    {
                      if(gepOperandSoA != NULL)
                      {
                        if(auto *GV = dyn_cast<GlobalVariable>(gepOperandSoA))
                        {
                          StructType* structure = cast<StructType>(GEP->getSourceElementType());
                          bool inConfirmed = findInConfirmedSoA(gepOperandSoA);
                          if(inConfirmed == false & (findInAoSoAList(gepOperandSoA) == false))
                            possibleGlobalsSoA.push_back(make_tuple(gepOperandSoA,originFunctionSoA,structure));
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }

// ------------------------------------------- END OF AOS DETECTION CODE -------------------------------------------

// ------------------------------------------- FINDING CORRECT ORIGIN OF EACH SOA -------------------------------------------

     // iterate through all SoA in 'confirmed' and find its original name and function where it has been used in a function call.
     for(int a = 0; a < confirmedSoA.size(); a++)
     {
       auto currConfirmed = confirmedSoA.at(a);
       Value* soa = get<0>(currConfirmed);
       Function* func = get<1>(currConfirmed);
       pair<Value*,Function*> result;
       bool found = false;
       string type = "dynamic";
       

      if(auto *SI = dyn_cast<StoreInst>(soa))
      {
        searchedFunctionCallsSoA.push_back(func);
        while(found == false)
        {
          result = getOriginSoA(func,soa,&M);

          if(auto *SI = dyn_cast<StoreInst>(result.first)) //no callinst found
          {
            searchedFunctionCallsSoA.pop_back();
            func = searchedFunctionCallsSoA.at(searchedFunctionCallsSoA.size() - 1);
          }
          else if(auto *CI = dyn_cast<CallInst>(result.first))
          {
              for(auto a = CI->arg_begin(); a != CI->arg_end(); a++)
              {
                Value* toFind = *a;
                if(auto *LI = dyn_cast<LoadInst>(toFind))
                  toFind = LI->getOperand(0);

                for(auto it = potentialArgumentsSoA.begin(); it != potentialArgumentsSoA.end(); it)
                {
                  Value* soa = get<0>(*it);
                  Function* func = get<1>(*it);

                  if(soa == toFind)
                  {
                    eraseFromPotentialArgumentsSoA(soa);
                    result.first = soa;
                    result.second = func;
                    type = get<2>(*it);
                    found = true;
                    break;
                  }
                  else
                    it++;
                }

                if(found == false)
                {
                  for(auto it = possibleGlobalsSoA.begin(); it != possibleGlobalsSoA.end(); it)
                  {
                    Value* soa = get<0>(*it);
                    Function* func = get<1>(*it);

                    if(soa == toFind)
                    {
                      eraseFromPossibleGlobalsSoA(soa);
                      result.first = soa;
                      result.second = func;
                      found = true;
                      break;
                    }
                    else
                      it++;
                  }
                }
            
                if(found == true)
                {
                  searchedCISoA.clear();
                  searchedFunctionCallsSoA.clear();
                  break;
                }
                else
                {
                  func = result.second;
                }
              }
            //}
          }
        }
      } 

      if(found == true)
      {
        get<0>(confirmedSoA.at(a)) = result.first;
        get<1>(confirmedSoA.at(a)) = result.second;
        get<2>(confirmedSoA.at(a)) = type;
      }
        
     }

      for(auto it = possibleGlobalsSoA.begin(); it != possibleGlobalsSoA.end(); it)
      {
        Value* soa = get<0>(*it);
        if(findInConfirmedSoA(soa))
          possibleGlobalsSoA.erase(it);
        else
          it++;
      }

      // ------------------------------------------- ORIGINS FOR EACH SOA FOUND -------------------------------------------

      int staticCountTemp = 0;
      int dynamicCountTemp = 0;

      for(auto it = confirmedSoA.begin(); it != confirmedSoA.end(); it++)
      {
        string type = get<2>(*it);
        if(type == "static")
          staticCountTemp++;
        else if(type == "dynamic")
          dynamicCountTemp++;
      }

      nonPointerCount = staticCountTemp;
      pointerCount = dynamicCountTemp;

    // ------------------------------------------- COLLECT STRUCT INFO FOR EACH SOA DATA STRUCTURE -------------------------------------------


      vector<pair<StructType*, bool>> checkedStructs;

      //iterate through all structs in the confirmedSoA list, and determine whether it contains a ptr field - which could make it recursive
      for(int i = 0; i < confirmedSoA.size(); i++)
      {

        StructType* structure = get<3>(confirmedSoA.at(i));

        /// for each used structure, store the size of each struct

        origStructSizes.insert(make_pair(structure,make_pair(TD->getTypeAllocSize(structure),TD->getTypeAllocSize(structure))));

        bool alreadyChecked = false;

        for(int j = 0; j < checkedStructs.size(); j++)
        {
          StructType* containedStruct = checkedStructs.at(j).first;
          bool hasPointerElem = checkedStructs.at(j).second;

          if(containedStruct == structure)
          {
            get<5>(confirmedSoA.at(i)) = hasPointerElem;
            alreadyChecked = true;
            break;
          }
        }

        if(alreadyChecked == true)
          continue;

        ArrayRef<Type*> elemArr = structure->elements();

        for(auto it = elemArr.begin(); it != elemArr.end(); it++)
        {
          Type* ty = const_cast<Type*>(*it);
          if(ty->isPointerTy())
          {
            errs()<<"Pointer found within struct\n";
            get<5>(confirmedSoA.at(i)) = true;
            break;
          }
        }
      }

        errs()<<"Size of potentialSoA list: "<<potentialSoA.size()<<"\n";
        for(int i = 0; i < potentialSoA.size(); i++)
        {
          std::string aos_str; 
          std::string func_str; 
          string struct_str;
          string type;

          raw_string_ostream soa(aos_str);
          raw_string_ostream func(func_str);
          raw_string_ostream stru(struct_str);
          get<0>(potentialSoA.at(i))->printAsOperand(soa);
          type = get<2>(potentialSoA.at(i));
          errs()<<i+1<<": "<<aos_str;
          Function* funcName = get<1>(potentialSoA.at(i));
          StructType* structure = get<3>(potentialSoA.at(i));
          if(funcName != NULL)
          {
            funcName->printAsOperand(func);
            errs()<<" - "<<type<<" type";
            if(structure != nullptr)
            {
              structure->print(stru);
              errs()<<" used in function: "<<func_str;
              errs()<<" with element: "<<struct_str<<"\n";
            }
            else
            {
              errs()<<" used in function: "<<func_str;
              errs()<<" with undefined element\n";
            }
          }
          else
          {
            errs()<<" - "<<type<<" global variable\n";
          }
          errs()<<"\n";
        }

        errs()<<"\nSize of possibleGlobalsSoA list: "<<possibleGlobalsSoA.size()<<"\n";
        for(int i = 0; i < possibleGlobalsSoA.size(); i++)
        {
          std::string aos_str; 
          std::string func_str; 
          raw_string_ostream soa(aos_str);
          raw_string_ostream func(func_str);
          get<0>(possibleGlobalsSoA.at(i))->printAsOperand(soa);
          errs()<<i+1<<": "<<aos_str;
          get<1>(possibleGlobalsSoA.at(i))->printAsOperand(func);
          errs()<<" used in function: "<<func_str<<"\n";
          errs()<<"\n";
        }

        errs()<<"\nNumber of static SoA data structures: " << nonPointerCount <<"\n";
        errs()<<"Number of dynamic SoA data structures: " << pointerCount <<"\n";

        errs()<<"Size of confirmed list: "<<confirmedSoA.size()<<"\n\n";
        for(int i = 0; i < confirmedSoA.size(); i++)
        {
          std::string aos_str; 
          std::string func_str; 
          string struct_str;
          bool isParam;
          bool hasPointerElem;

          string type = get<2>(confirmedSoA.at(i));
          raw_string_ostream soa(aos_str);
          raw_string_ostream func(func_str);
          raw_string_ostream stru(struct_str);
          get<0>(confirmedSoA.at(i))->print(soa);
          errs()<<i+1<<": "<<aos_str;

          Function* funcName = get<1>(confirmedSoA.at(i));
          StructType* structure = get<3>(confirmedSoA.at(i));
          isParam = get<4>(confirmedSoA.at(i));
          hasPointerElem = get<5>(confirmedSoA.at(i));

          if(funcName != NULL)
          {
            funcName->printAsOperand(func);
            errs()<<" - "<<type<<" SoA used in function: "<<func_str;
            if(structure != nullptr)
            {
              struct_str = structure->getName();
              errs()<<" with element: "<<struct_str<<"\n";
            }
            else
            {
              errs()<<" with undefined element"<<"\n";
            }

            if(isParam == true)
            {
              errs()<<" - used as function argument\n";
            }
            if(hasPointerElem == true)
            {
              errs()<<" - uses struct that contains a pointer field\n";
            }
          }
          else
          {
            errs()<<" - global "<<type<<" SoA";

            if(structure != nullptr)
            {
              struct_str = structure->getName();
              errs()<<" with element: "<<struct_str<<"\n";
            }
            else
            {
              errs()<<" with undefined element"<<"\n";
            }

            if(isParam == true)
            {
              errs()<<" - used as function argument\n";
            }
            if(hasPointerElem == true)
            {
              errs()<<" - uses struct that contains a pointer field\n";
            }
          }
          errs()<<"\n";
        }

        errs()<<"\n----------------------- END OF SOA DETECTION -----------------------\n";
        //Set to ::all() if IR is unchanged, otherwise ::none()
        return PreservedAnalyses::none();
    };
};
}