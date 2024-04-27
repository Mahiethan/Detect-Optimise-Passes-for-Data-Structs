//CONDITION: for AoS, the array must be populated with some data, either explicitly in the main function or via a function
//empty AoS will not be detected as 'confirmed'

#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include "./detectAoS.h" // required to share the 'confirmed' AoS list with the optimisation passes

#include <algorithm>
#include <string>
#include <deque>

using namespace llvm;
using namespace std;

vector<tuple<Value*,Function*,string,StructType*>> potential; // stores list of pointers that potentially be storing an AoS
vector<tuple<Value*,Function*,string,StructType*>> potentialArguments; // stores list of arguments used in function calls that could be AoS
vector<tuple<Value*,StoreInst*,string>> argStores; // stores list of StoreInst that stores the arguments within the function body
vector<tuple<Value*,Function*,StructType*>> possibleGlobals; // stores list of global variables that potentially be an AoS
set<tuple<int,string,vector<int>,Value*,Function*,string>> calledFunction; // stores pair of function name and used argument index of pointer (if any)
int calledNum = 0; // index used to store each called function uniquely in a set.

map<string,set<string>> functionStream; //for each function, stores names of functions that may be called within in

Function* originFunction = NULL; //temporary variable used to store original function of current AoS being inspected

int staticCount = 0; // total number of found static AoS
int dynamicCount = 0; // total number of found dynamic AoS
bool mallocFlag = false; // flag used to check for StoreInst right after a malloc() function call is found


// temporary data structures used to obtain original AoS names and functions (if they are used in function calls)
set<CallInst*> searchedCI; 
vector<Function*> searchedFunctionCalls;

// ------------------------------------------- START OF HELPER FUNCTIONS -------------------------------------------

// find origin of a function call
pair<Value*,Function*> getOrigin(Function* toSearch, Value* call, Module* M)
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
              Function* calledFunction = CI->getCalledFunction();
              Function* parentFunction = &F;
              if(calledFunction == toSearch & searchedCI.find(CI) == searchedCI.end())
              {
                searchedCI.insert(CI);
                searchedFunctionCalls.push_back(parentFunction);
                return make_pair(CI,parentFunction);
              }  
            }
           }
        }
      }
    return result;
  }

// obtain statically declared AoS
void detectStaticAoS(AllocaInst* AI)
{
  Type* returnType = AI->getAllocatedType();

  //get type as a string
  std::string type_str; 
  raw_string_ostream to_str(type_str);
  returnType->print(to_str);

  if(AI->isArrayAllocation()) //identifying AoS with size given as a variable - if the alloca instruction has a size attribute that is not 1 (i.e. not a single allocation of the type)
  {
      if(returnType->isStructTy()) //if the allocated type is a struct
      {
        Value* AoS = cast<Instruction>(AI); //store as Value in vector
        StructType* structure = cast<StructType>(returnType);
        
        potential.push_back(make_tuple(AoS,originFunction,"static",structure));
      }
  }
  else if(type_str.find("struct") != std::string::npos & type_str.find("[") != std::string::npos & type_str.find("]") != std::string::npos) //identifying AoS with size given as int literal - if type contains word "struct"
  {
      if(type_str.find("[0 x") == std::string::npos) //if allocated size is not 0
      {
      //allocation size can be 1 or more
      if(auto *AT = dyn_cast<ArrayType>(AI->getAllocatedType()))
      {
        Type* elem = AT->getArrayElementType();
        int num = AT->getArrayNumElements();
        if(num > 0)
        {
          if(auto *ST = dyn_cast<StructType>(elem))
          {
              Value* AoS = cast<Instruction>(AI); //store as Value in vector
              StructType* structure = cast<StructType>(elem);
              potential.push_back(make_tuple(AoS,originFunction,"static",structure));
          }
        }
      }
      }
  }
}

// check whether an AoS is found in the 'confirmed' list
bool findInConfirmed(Value* val)
{
  for(auto it = confirmed.begin(); it != confirmed.end(); it)
  {
    Value* aos = get<0>(*it);
  
    if(aos == val)
    {
      printf("Found in confirmed\n");
      return true;
    }
    else
      it++;
  }
  return false;
}

// erase an AoS from the 'confirmed' list, if found.
StructType* eraseFromConfirmed(Value* val)
{
  for(auto it = confirmed.begin(); it != confirmed.end(); it)
  {
    Value* aos = get<0>(*it);
    string type = get<2>(*it);
    StructType* structure = get<3>(*it);
    std::string aos_string; 
    raw_string_ostream aos_stream(aos_string);
    aos->printAsOperand(aos_stream);
    if(aos == val)
    {
      confirmed.erase(it);
    if(type == "static")
          staticCount--;
    else if(type == "dynamic")
      dynamicCount--;

      return structure;
    }
    else
    {
    it++;
    }
  }
  return nullptr;
}

// erase the global AoS from the 'possibleGlobals' list if found
StructType* eraseFromPossibleGlobals(Value* val)
{
  for(auto it = possibleGlobals.begin(); it != possibleGlobals.end(); it)
  {
    Value* aos = get<0>(*it);
    StructType* structure = get<2>(*it);

    if(aos == val)
    {
      possibleGlobals.erase(it);
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
bool checkIfRecursive(string nextFunction, string currentFunction)
{
  if(functionStream.find(nextFunction) != functionStream.end())
  {
    set<string> nextList = functionStream.find(nextFunction)->second;
    if(nextList.find(currentFunction) != nextList.end())
      return true;
  }
  return false;
}

//function used to add an AoS to potential vector
void getPotential(Instruction* I) 
{
  if(auto *SI = dyn_cast<StoreInst>(I))
  {
    Value* operand = SI->getOperand(1); //get second operand of this store instruction
    
    std::string aos_string; 
    raw_string_ostream aos_stream(aos_string);
    operand->printAsOperand(aos_stream);


    bool foundInPossibleGlobals = false;
    bool foundInPotential = false;
    Value* aos;
    Function* func;
    for(auto it = possibleGlobals.begin(); it != possibleGlobals.end(); it)
    {
      aos = get<0>(*it);
      func = get<1>(*it);

      if(aos == operand)
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
      StructType* structure = eraseFromPossibleGlobals(operand);
      if(!findInConfirmed(operand))
      {
        confirmed.push_back(make_tuple(aos,func,"dynamic",structure,false,false,"AoS"));
        dynamicCount++;
      }
    }
    else
    {
      for(auto it = potential.begin(); it != potential.end(); it)
      {
        aos = get<0>(*it);
        func = get<1>(*it);

        if(aos == operand & func == originFunction)
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
        StructType* structure = eraseFromConfirmed(operand); //remove from confirmed list if it exists before adding it to potential
        potential.push_back(make_tuple(operand,originFunction,"dynamic",structure));
      }
    }
  }  
  mallocFlag = false; //disable flag - don't search for StoreInst after malloc() function call
}

// erase an AoS from the 'potential' list if found
tuple<llvm::Value *, llvm::Function *, std::string, llvm::StructType *> eraseFromPotential(Value* val)
{
  for(auto it = potential.begin(); it != potential.end(); it)
  {
    Value* aos = get<0>(*it);
    StructType* structure = get<3>(*it);
    tuple<Value*,Function*,string,StructType*> elem = *it;

    if(aos == val)
    {
      potential.erase(it);
      return elem;
    }
    else
    {
    it++;
    }
  }
  return make_tuple(nullptr,nullptr,"",nullptr);
}

// erase an AoS function argument from the 'potentialArguments' list
void eraseFromPotentialArguments(Value* val)
{
  for(auto it = potentialArguments.begin(); it != potentialArguments.end(); it)
  {
    Value* aos = get<0>(*it);
    StructType* structure = get<3>(*it);
    auto elem = *it;

    if(aos == val)
    {
      potentialArguments.erase(it);
    }
    else
    {
    it++;
    }
  }
}

// erase an StoreInst from 'argStores' if found
pair<StoreInst*,string> eraseFromArgStores(Value* val)
{
  for(auto it = argStores.begin(); it != argStores.end(); it)
  {
    Value* aos = get<0>(*it);
    StoreInst* store = get<1>(*it);
    string type = get<2>(*it);
    std::string aos_string; 
    raw_string_ostream aos_stream(aos_string);
    aos->printAsOperand(aos_stream);

    std::string val_string; 
    raw_string_ostream val_stream(val_string);
    val->printAsOperand(val_stream);

    if(aos == val)
    {
      argStores.erase(it);
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
Value* gepOperand = NULL;
Value* calledAoS = NULL;
Function* paramFunction = NULL;

// check whether a GEP instruction is accessing a structure element from an array
bool checkGEP(GetElementPtrInst *gep, Value* aos, bool isParam, string type)
{
    Value* operand = gep->getOperand(0);
    StructType* gepStruct = nullptr;
    bool isAoSoA = false;

    //getting operand as string
    std::string op_string; 
    raw_string_ostream ops(op_string);
    operand->print(ops);

    if(auto *ST = dyn_cast<StructType>(gep->getResultElementType()))
    {
      gepStruct = ST;
      // //check if struct is in toFind and set bool - this checks whether an AoSoA is found
      if(toFind.find(gepStruct) != toFind.end())
        isAoSoA = true;

    }
    else if(auto *ST = dyn_cast<StructType>(gep->getSourceElementType()))
    {
      gepStruct = ST;
      //check if struct is in toFind and set bool - this checks whether an AoSoA is found
      if(toFind.find(gepStruct) != toFind.end())
        isAoSoA = true;
    }
    else
      return false;

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
    
    if(auto *GV = dyn_cast<GlobalVariable>(operand)) //GEP operating on a global AoS
    {
      std::string ptr_string; 
      raw_string_ostream ptr(ptr_string);
      aos->print(ptr);

      if(op_string == ptr_string)
      {
        if(type == "static")
          staticCount++;
        else if(type == "dynamic")
          dynamicCount++;

        string AoSType = "AoS";
        if(isAoSoA)
          AoSType = "AoSoA";

        // output not needed
        StructType* structure = get<3>(eraseFromPotential(aos));
        if(structure == nullptr)
          structure = eraseFromPossibleGlobals(aos);
        confirmed.push_back(make_tuple(aos,originFunction,type,gepStruct,false,false,AoSType));
        return true;
      }
    }
    else if(auto *AI = dyn_cast<AllocaInst>(operand))
    {
      std::string ptr_string; 
      raw_string_ostream ptr(ptr_string);
      aos->print(ptr);

      if(op_string == ptr_string) //if loaded operand is one of the malloc-ed arrays detected earlier
      {
        if(type == "static")
          staticCount++;
        else if(type == "dynamic")
          dynamicCount++;

        string AoSType = "AoS";
        if(isAoSoA)
          AoSType = "AoSoA";

        // output not needed
        StructType* structure =  get<3>(eraseFromPotential(aos));
        if(structure == nullptr)
          structure = eraseFromPossibleGlobals(aos);

        if(isParam)
        {
          Value* storedAoS = eraseFromArgStores(aos).first;
          confirmed.push_back(make_tuple(storedAoS,originFunction,type,gepStruct,true,false,AoSType));
        }
        else
          confirmed.push_back(make_tuple(aos,originFunction,type,gepStruct,false,false,AoSType));

        return true;
      }
      gepOperand = operand;
    }
    else if(auto *OI = dyn_cast<LoadInst>(operand)) //check if instruction is a Load instruction
    {
      Value* operand = OI->getOperand(0); //get operand that is being loaded in

      std::string op_string; 
      raw_string_ostream ops(op_string);
      operand->print(ops);      

      std::string ptr_string; 
      raw_string_ostream ptr(ptr_string);
      aos->print(ptr);

      if(op_string == ptr_string) //if loaded operand is one of the malloc-ed arrays detected earlier
      {
       if(type == "static")
          staticCount++;
        else if(type == "dynamic")
          dynamicCount++;

        string AoSType = "AoS";
        if(isAoSoA)
          AoSType = "AoSoA";

        // output not needed
        StructType* structure =  get<3>(eraseFromPotential(aos));
        if(structure == nullptr)
          structure = eraseFromPossibleGlobals(aos);

        if(isParam)
        {
          Value* storedAoS = eraseFromArgStores(aos).first;
          confirmed.push_back(make_tuple(storedAoS,originFunction,type,gepStruct,true,false,AoSType));
        }
        else
          confirmed.push_back(make_tuple(aos,originFunction,type,gepStruct,false,false,AoSType));
      
        return true;
      }
      gepOperand = operand;

    }
  
  return false;
}

// obtain the function that has been called using the CallInst
void getCalledFunctions(CallInst* CI, Function* orig)
{
  vector<int> indices;
  Value* aos;
  Value* paramAoS;
  string type = "undefined";
  int index = 0;

  CI->setTailCall(false);

  Function* f = CI->getCalledFunction();
  string funcName = f->getName().str();

  if(funcName == orig->getName() | checkIfRecursive(funcName,orig->getName().str())) //function call may be recursive
  {
    return; //return nothing
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

    
    for(int i = 0; i < potential.size(); i++)
    {
      aos = get<0>(potential.at(i));
      std::string value_string; 
      raw_string_ostream aos_stream(value_string);
      aos->printAsOperand(aos_stream);

      if(value_string == op_string)
      {
        indices.push_back(index);
        //add to new list: potentialArguments
        tuple<Value*,Function*,string,StructType*> elem = eraseFromPotential(aos);
        type = get<2>(elem);
        potentialArguments.push_back(elem);
        detect = true;
        paramAoS = aos;

        break;
      }
    }
    if(detect == false)
    {
      for(int i = 0; i < argStores.size(); i++)
      {
        aos = get<0>(argStores.at(i));
        std::string value_string; 
        raw_string_ostream aos_stream(value_string);
        aos->printAsOperand(aos_stream);

        if(value_string == op_string)
        {
          detect = true;
          indices.push_back(index);
          eraseFromArgStores(aos);
          paramAoS = aos;

          break;
        }
      }
    }

    if(detect == false) //add to possible globals
    {
      if(auto *GV = dyn_cast<GlobalVariable>(operand)) //if a function call has a global parameter not yet malloced, add it to possibleGlobals
      {
        bool inConfirmed = findInConfirmed(operand);
        if(inConfirmed == false)
          possibleGlobals.push_back(make_tuple(operand,f,nullptr));
        detect = true;
      }
    }
  
    index++;
  }

  calledFunction.insert(std::make_tuple(calledNum,funcName,indices,paramAoS,orig,type));
  calledNum++;
}


// obtain the StoreInst that stores the potential AoS argument 
vector<tuple<Value*,StoreInst*,string>> getArgumentStores(Function *F, vector<int> indices,string type)
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

// ------------------------------------------- START OF AOS DETECTION CODE -------------------------------------------


namespace {

struct detectAoS : public PassInfoMixin<detectAoS> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {

        errs()<<"\n-------------------------- AOS DETECTION --------------------------\n\n";

        detectAoSCalled = true;

        DataLayout* TD = new DataLayout(&M); //to get size and layout of structs

        vector<Value*> possibleAoSList; //vector which stores pointer locations of possible AoS - these need to be inspected and confirmed

        vector<Value*> AoS_Values; //vector which stores pointer locations of possible AoS - these need to be inspected and confirmed

        // checking all global variables
        for(GlobalVariable& gv : M.globals())
        {
          if(gv.getName() == "permitStructSplittingFlag") //if this global variable flag created in splitAoS is detected, set this flag to false, else true
            permitStructSplitting = false;
          else
            permitStructSplitting = true;

           std::string type_str; 
           raw_string_ostream to_str(type_str);
           gv.print(to_str);

          /// Search for static AoS within all GlobalVariables of the program
           
          if(type_str.find("struct") != std::string::npos & type_str.find("[") != std::string::npos & type_str.find("]") != std::string::npos) //identifying AoS with size given as int literal
          {
            if(type_str.find("[0 x") == std::string::npos) //if allocated size is not 0
            {
              Constant* constValue = nullptr;

              if(gv.hasInitializer())
                constValue = gv.getInitializer(); //get the initializer
              else
                continue;

              Type* t = constValue->getType(); //get the type of the global
              string type_str;
              raw_string_ostream tstr(type_str);
              t->print(tstr);

              Type* globalElem;

              if(auto *AT = dyn_cast<ArrayType>(t)) //if global is an array, gets it element type
                globalElem = AT->getArrayElementType();

              if(auto *ST = dyn_cast<StructType>(globalElem))
              {
                Value* AoS = cast<Value>(&gv); //store as Value in vector

                potential.push_back(make_tuple(AoS,originFunction,"static",ST)); //add to 'potential' vector
              }
            }
          }
        }
        
        // flag set to true if main() function has been searched through
        bool checkMain = false;

        for (auto &F : M) //iterate through all functions in the Module and print their names
        { 
            if(checkMain == true) //if already searched through main()
              break;
            if(F.getName().find("main") != std::string::npos)  
            {
              originFunction = &F; // keep track of the current function to be used if an AoS is found within it

              for (auto &B : F)  //iterate through all blocks in the 'main' function
              { 
                  for (auto &I : B) //iterate through all instructions in the 'main' block
                  { 
                    if(mallocFlag == true) //if a MallocInst was found in previous instruction, the current instruction should be a StoreInst.
                    {
                      getPotential(&I); //process this StoreInst to be added to the 'potential' vector
                    }
                    else if(auto *SI = dyn_cast<StoreInst>(&I)) //finding global static AoS used in a GEP operation
                    {
                      bool found = false;
                      Value* operand = SI->getOperand(1);
                      std::string op_str; 
                      raw_string_ostream to_str(op_str);
                      operand->printAsOperand(to_str);

                      // get the AoS that is being operated on

                      Value* loadedAoS = SI->getOperand(0);

                      if(auto *LI = dyn_cast<LoadInst>(SI->getOperand(0))) // there might be another LoadInst so obtain AoS from here
                      {
                        loadedAoS = LI->getOperand(0);
                      }

                      /*
                        if the loadad AoS is found in potential, this static AoS is detected and should be placed in 'confirmed'
                      */
                      for(auto it = potential.begin(); it != potential.end(); it++)
                      {
                        Value* aos = get<0>(*it);
                        std::string ptr_string; 
                        raw_string_ostream ptr(ptr_string);
                        aos->printAsOperand(ptr);

                        if((op_str.find("getelementptr") != std::string::npos) & (op_str.find(ptr_string) != std::string::npos))
                        {
                          StructType* structure =  get<3>(eraseFromPotential(aos));
                          staticCount++;
                          confirmed.push_back(make_tuple(aos,originFunction,"static",structure,false,false,"AoS"));
                          found = true;
                          break;
                        }
                      }

                      /*
                        if the loadad AoS is found in potential, remove it from 'confirmed' and add it back to 'potential'
                        this is because the AoS may be reinitialed/emptied out within the same scope so it no longer contains data - this no longer an AoS
                      */  
                      if(found == false)
                      {
                        for(auto it = potential.begin(); it != potential.end(); it++)
                        {
                          Value* aos = get<0>(*it);
                          string type = get<2>(*it);
                          StructType* structure = get<3>(*it);

                          if(loadedAoS == aos) //a potential AoS could be stored into an empty pointer - add this pointer to 'potential' list
                          {
                            eraseFromPossibleGlobals(SI->getOperand(1));
                            potential.push_back(make_tuple(SI->getOperand(1),originFunction,type,structure));
                            found = true;
                            break;
                          }
                        }
                        // similarly a 'confirmed' AoS can be stored to an empty pointer so add it to 'confirmed'
                        for(auto it = confirmed.begin(); it != confirmed.end(); it++)
                        {
                          Value* aos = get<0>(*it);
                          string type = get<2>(*it);
                          StructType* structure = get<3>(*it);
                          string aosType = get<6>(*it);

                          if(loadedAoS == aos) //a potential AoS could be stored into an empty pointer - add this pointer to 'potential' list
                          {
                            eraseFromPossibleGlobals(SI->getOperand(1));
                            confirmed.push_back(make_tuple(SI->getOperand(1),originFunction,type,structure,false,false,aosType));
                            found = true;
                            break;
                          }
                        }
                      }
                    }
                    else if(auto *AI = dyn_cast<AllocaInst>(&I)) //an AllocaInst could be a static AoS
                      detectStaticAoS(AI);
                    else if(auto *CI = dyn_cast<CallInst>(&I)) //a CallInst could be a malloc()/calloc() function creating a dynamic AoS or a function call using a potential AoS
                    {
                      CI->setTailCall(false); //avoid tail calls - to maintain compatibility with other passes

                      Function* f = CI->getCalledFunction(); //get hte called function
                      string funcName = f->getName().str();

                      if(funcName == "malloc" | funcName == "calloc") //if the function is malloc()/calloc(), a ptr is being allocated data which could be an AoS so set the following flag to true
                        mallocFlag = true;
                      else if(funcName != "free") //for any functions other than malloc(), calloc() or free(), store the function to later inspection of any newly created AoS or data accesses/writes to 'potential' AoS
                      {
                        if(auto *SI = dyn_cast<StoreInst>(I.getNextNode()))
                        {
                          if(f->getReturnType()->isPointerTy() & (SI->getValueOperand() == CI))
                          {
                            potential.push_back(make_tuple(SI->getPointerOperand(),originFunction,"dynamic",nullptr));
                          }
                        }
                        getCalledFunctions(CI,&F);
                      }
                    }
                    else if(auto *GEP = dyn_cast<GetElementPtrInst>(&I)) //a GEPInst may be working on a AoS found in potential/static global AoS
                    {
                      bool found = false;
                      Value* aos;
                      string type;

                      for(auto it = potential.begin(); it != potential.end(); it++)
                      {
                        aos = get<0>(*it);
                        type = get<2>(*it);
                        std::string ptr_string; 
                        raw_string_ostream ptr(ptr_string);
                        aos->printAsOperand(ptr);

                        if(checkGEP(GEP,aos,false,type) == true) //call this function to check if GEP is accessing a 'potential' AoS
                        {
                          found = true;
                          eraseFromPotential(aos);
                          break;
                        }
                      }
                      if(found == false) //if the AoS is not in 'potential', it may already be found and present in 'confirmed' OR it is operating on a global AoS ptr declared elsewhere (in another function)
                      {
                        if(gepOperand != NULL) //stores recent AoS that has been accessed in a GEPInst.
                        {
                          if(auto *GV = dyn_cast<GlobalVariable>(gepOperand))
                          {
                            StructType* structure;
                            if(GEP->getResultElementType()->isStructTy())
                              structure = cast<StructType>(GEP->getResultElementType());
                            else if(GEP->getSourceElementType()->isStructTy())
                              structure = cast<StructType>(GEP->getSourceElementType());
                            else
                              structure = nullptr;
                            bool inConfirmed = findInConfirmed(gepOperand);
                            if(inConfirmed == false) //if the AoS is not found in 'confirmed', add it to 'possibleGlobals' vector instead - stores global AoS pointers that may be populated later
                              possibleGlobals.push_back(make_tuple(gepOperand,originFunction,structure));
                          }
                        }
                      }

                    }
                  }
              checkMain = true; // main() function has been searched through completely
              }
            }
        }

      // Now go through all called functions and search for any newly declared AoS values or search for any 'potential' AoS values that are being populated
      for(auto it : calledFunction)
      {
        tuple<int,string,vector<int>,Value*,Function*,string> searchFunc = it;

        string type = get<5>(searchFunc);

        vector<int> argIndices = get<2>(searchFunc);

        calledAoS = get<3>(searchFunc);  //get AoS that was used as parameter in function call

        paramFunction = get<4>(searchFunc);  //get the whole block of called function

        string currentFuncName = get<1>(searchFunc);

        //Don't iterate through all functions, just search through the found function 'paramFunction'

        for (auto &F : M) //iterate through all functions in the Module and print their names
        { 
          if(F.getName() == currentFuncName)
          {
            set<string> next;
            functionStream.insert(make_pair(F.getName().str(),next));

            originFunction = &F;
            // errs()<<"Searching "<<get<0>(searchFunc)<<"\n";
            if(argIndices.size() != 0)
            {
              argStores = getArgumentStores(&F,argIndices,type);
              // errs()<<"compare indices "<<argIndices.size()<<" to argStores "<<argStores.size()<<"\n";
            }

            for(auto &B : F) //iterate through all BasicBlocks and Instructions of the called function
            {
              for(auto &I : B) //perform same inspection for AoS detection as done previously in the main() function
              {
                if(mallocFlag == true)
                {
                  getPotential(&I);
                }
                else if(auto *SI = dyn_cast<StoreInst>(&I)) //find global static AoS used in a GEP operation
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

                  for(auto it = potential.begin(); it != potential.end(); it++)
                  {
                    Value* aos = get<0>(*it);
                    std::string ptr_string; 
                    raw_string_ostream ptr(ptr_string);
                    aos->printAsOperand(ptr);

                    if((op_str.find("getelementptr") != std::string::npos) & (op_str.find(ptr_string) != std::string::npos))
                    {
                      StructType* structure =  get<3>(eraseFromPotential(aos));
                      staticCount++;
                      confirmed.push_back(make_tuple(aos,originFunction,"static",structure,false,false,"AoS"));
                      found = true;
                      break;
                    }
                  }
                  // similarly a 'confirmed' AoS can be stored to an pointer so add the pointer to 'confirmed'
                  if(!found)
                  {
                    for(auto it = confirmed.begin(); it != confirmed.end(); it++)
                    {
                      Value* aos = get<0>(*it);
                      string type = get<2>(*it);
                      StructType* structure = get<3>(*it);
                      string aosType = get<6>(*it);

                      if(loadedAoS == aos)
                      {
                        eraseFromPossibleGlobals(SI->getOperand(1));
                        confirmed.push_back(make_tuple(SI->getOperand(1),originFunction,type,structure,false,false,aosType));
                        found = true;
                        break;
                      }
                    }
                  }
                }
                else if(auto *AI = dyn_cast<AllocaInst>(&I))
                  detectStaticAoS(AI);
                else if(auto *CI = dyn_cast<CallInst>(&I))
                {
                  CI->setTailCall(false);

                  Function* f = CI->getCalledFunction();
                  string funcName = f->getName().str();

                  if(funcName == "malloc" | funcName == "calloc")
                  {
                    mallocFlag = true;
                  }
                  else if(funcName != "free")
                  {
                    //if next inst is a store that using this call AND
                    //return type of called function is a ptr
                      //add the callisnt ptr operand to potential

                    if(auto *SI = dyn_cast<StoreInst>(I.getNextNode()))
                    {
                      if(f->getReturnType()->isPointerTy() & (SI->getValueOperand() == CI))
                      {
                        potential.push_back(make_tuple(SI->getPointerOperand(),originFunction,"dynamic",nullptr));
                      }
                    }

                    if(functionStream.find(funcName) != functionStream.end()) //store all of the next functions
                    {
                      if(functionStream.find(currentFuncName) != functionStream.end()) //store all of the next functions
                      {
                        functionStream.find(currentFuncName)->second.insert(funcName);
                        set<string> toAdd = functionStream.find(funcName)->second;
                        for(auto it : toAdd)
                        {
                          functionStream.find(currentFuncName)->second.insert(it);
                        }
                      }
                    }
                    getCalledFunctions(CI,&F);
                  }
                }
                else if(auto *GEP = dyn_cast<GetElementPtrInst>(&I))
                {
                  bool found = false;
                  Value* aos;
                  string type;
                  for(auto it = potential.begin(); it != potential.end(); it++)
                  {
                    aos = get<0>(*it);
                    type = get<2>(*it);
                    if(checkGEP(GEP,aos,false,type) == true)
                    {
                      found = true;
                      eraseFromPotential(aos);
                      break;
                    }
                  }

                  // within called functions, search whether a potential AoS arguemet in 'argStores' is being accessed/written to in a GEPInst
                  for(auto it = argStores.begin(); it != argStores.end(); it++)
                  {
                    aos = get<0>(*it);
                    type = get<2>(*it);
                    if(checkGEP(GEP,aos,true,type) == true)
                    {
                      found = true;
                      break;
                    }
                  }

                  if(found == false)
                  {
                    if(gepOperand != NULL)
                    {
                      if(auto *GV = dyn_cast<GlobalVariable>(gepOperand))
                      {
                        StructType* structure;
                        if(GEP->getResultElementType()->isStructTy())
                          structure = cast<StructType>(GEP->getResultElementType());
                        else if(GEP->getSourceElementType()->isStructTy())
                          structure = cast<StructType>(GEP->getSourceElementType());
                        else
                          structure = nullptr;
                        bool inConfirmed = findInConfirmed(gepOperand);
                        if(inConfirmed == false)
                          possibleGlobals.push_back(make_tuple(gepOperand,originFunction,structure));
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

// ------------------------------------------- FINDING CORRECT ORIGIN OF EACH AOS -------------------------------------------

     
     // iterate through all AoS in 'confirmed' and find its original name and function where it has been used in a function call.
     for(int a = 0; a < confirmed.size(); a++)
     {
       auto currConfirmed = confirmed.at(a);
       Value* aos = get<0>(currConfirmed);
       Function* func = get<1>(currConfirmed);
       pair<Value*,Function*> result;
       bool found = false;
       string type = "dynamic";
       

      if(auto *SI = dyn_cast<StoreInst>(aos))
      {
        searchedFunctionCalls.push_back(func);
        while(found == false)
        {
           result = getOrigin(func,aos,&M);

          if(auto *SI = dyn_cast<StoreInst>(result.first)) //no CallInst found
          {
            searchedFunctionCalls.pop_back();
            func = searchedFunctionCalls.at(searchedFunctionCalls.size() - 1);
          }
          else if(auto *CI = dyn_cast<CallInst>(result.first))
          {
            for(auto a = CI->arg_begin(); a != CI->arg_end(); a++)
            {
              Value* toFind = *a;
              if(auto *LI = dyn_cast<LoadInst>(toFind))
                toFind = LI->getOperand(0);

              for(auto it = potentialArguments.begin(); it != potentialArguments.end(); it)
              {
                Value* aos = get<0>(*it);
                Function* func = get<1>(*it);

                if(aos == toFind)
                {
                  eraseFromPotentialArguments(aos);
                  result.first = aos;
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
                for(auto it = possibleGlobals.begin(); it != possibleGlobals.end(); it)
                {
                  Value* aos = get<0>(*it);
                  Function* func = get<1>(*it);

                  if(aos == toFind)
                  {
                    eraseFromPossibleGlobals(aos);
                    result.first = aos;
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
                searchedCI.clear();
                searchedFunctionCalls.clear();
                break;
              }
              else
              {
                func = result.second;
              }
            }
          }
        }
      } 

      if(found == true)
      {
        get<0>(confirmed.at(a)) = result.first;
        get<1>(confirmed.at(a)) = result.second;
        get<2>(confirmed.at(a)) = type;
      }
        
     }

      for(auto it = possibleGlobals.begin(); it != possibleGlobals.end(); it)
      {
        Value* aos = get<0>(*it);
        if(findInConfirmed(aos))
          possibleGlobals.erase(it);
        else
          it++;
      }

      // remove any SoA that may be accidently found and stored in 'confirmed' vector
      for(auto it = confirmed.begin(); it != confirmed.end(); it)
      {
          Value* confirmedAoS = get<0>(*it);
          bool found = false;
          for(auto itr : confirmedSoA)
          {
            Value* confirmedSoA = get<0>(itr);
            if(confirmedAoS == confirmedSoA)
            {
              confirmed.erase(it);
              found = true;
              break;
            }
          }
          if(!found)
            it++;
      }

     //a malloc-ed() ptr with only a single struct size is not an AoS, so remove it
     for(auto it = confirmed.begin(); it != confirmed.end(); it)
     {
        Value* aos = get<0>(*it);
        string type = get<2>(*it);
        StructType* structure = get<3>(*it);

        int structSize = TD->getTypeAllocSize(structure);

        if(type == "static")
        {
          it++;
        }
        else
        {
            bool isRemoved = false;
            
            for(auto uses : aos->users())
            {
              Instruction* use = nullptr;
              if(isa<Instruction>(uses))
                use = cast<Instruction>(uses);
              else
                continue;

              if(auto *SI = dyn_cast<StoreInst>(use))
              {
                if(auto *CI = dyn_cast<CallInst>(SI->getPrevNode()))
                {
                  if(CI->getCalledFunction()->getName().str() == "malloc")
                  {
                    Value* size = CI->getOperand(0); //only one function argument for malloc()

                    if(auto *CO = dyn_cast<ConstantInt>(size))
                    {
                      int constIntValue = CO->getSExtValue();

                      if(constIntValue == structSize)
                      {
                        confirmed.erase(it);
                        isRemoved = true;
                        break;
                      }
                    }
                  }
                }
              
            }
          }
          if(!isRemoved)
            it++;
        }
     }

     // ------------------------------------------- ORIGINS FOR EACH AOS FOUND -------------------------------------------

      int staticCountTemp = 0;
      int dynamicCountTemp = 0;

      // count the number of "static" and "dynamic" AoS data structures in the "confirmed" vector
      for(auto it = confirmed.begin(); it != confirmed.end(); it++)
      {
        string type = get<2>(*it);
        if(type == "static")
          staticCountTemp++;
        else if(type == "dynamic")
          dynamicCountTemp++;
      }

      staticCount = staticCountTemp;
      dynamicCount = dynamicCountTemp;

// ------------------------------------------- COLLECT STRUCT INFO FOR EACH AOS DATA STRUCTURE -------------------------------------------

      vector<pair<StructType*, bool>> checkedStructs;

      // iterate through all structs in the confirmed list, and determine whether it contains a ptr field - which could make it recursive
      // this is later used to determine whether struct splitting can be applied or not

      for(int i = 0; i < confirmed.size(); i++)
      {
        StructType* structure = get<3>(confirmed.at(i));

        // for each used structure, store the size of each struct
        origStructSizes.insert(make_pair(structure,make_pair(TD->getTypeAllocSize(structure),TD->getTypeAllocSize(structure))));

        bool alreadyChecked = false;

        for(int j = 0; j < checkedStructs.size(); j++) // multiple AoS data structures may have the same struct elements, so don't check these again
        {
          StructType* containedStruct = checkedStructs.at(j).first;
          bool hasPointerElem = checkedStructs.at(j).second;

          if(containedStruct == structure)
          {
            get<5>(confirmed.at(i)) = hasPointerElem;
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
            get<5>(confirmed.at(i)) = true; // this is later checked when applying struct splitting/peeling
            break;
          }
        }
      }

// ------------------------------------------- PRINTING ALL AOS DATA STRUCTURES -------------------------------------------

      // print out pointers / empty AoS stored in 'potential' list

      errs()<<"Size of potential list: "<<potential.size()<<"\n";
      for(int i = 0; i < potential.size(); i++)
      {
        std::string aos_str; 
        std::string func_str; 
        string struct_str;
        string type;
      
        raw_string_ostream aos(aos_str);
        raw_string_ostream func(func_str);
        raw_string_ostream stru(struct_str);
        get<0>(potential.at(i))->printAsOperand(aos);
        type = get<2>(potential.at(i));
        errs()<<i+1<<": "<<aos_str;
        Function* funcName = get<1>(potential.at(i));
        StructType* structure = get<3>(potential.at(i));
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

      // print out empty global AoS stored in 'possibleGlobals'

      errs()<<"\nSize of possibleGlobals list: "<<possibleGlobals.size()<<"\n";
      for(int i = 0; i < possibleGlobals.size(); i++)
      {
        std::string aos_str; 
        std::string func_str; 
        raw_string_ostream aos(aos_str);
        raw_string_ostream func(func_str);
        get<0>(possibleGlobals.at(i))->printAsOperand(aos);
        errs()<<i+1<<": "<<aos_str;
        get<1>(possibleGlobals.at(i))->printAsOperand(func);
        errs()<<" used in function: "<<func_str<<"\n";
        errs()<<"\n";
      }

      errs()<<"\nNumber of static AoS data structures: " << staticCount <<"\n";
      errs()<<"Number of dynamic AoS data structures: " << dynamicCount <<"\n";

      // print out found AoS data structures in 'confirmed' list

      errs()<<"Size of confirmed list: "<<confirmed.size()<<"\n\n";
      for(int i = 0; i < confirmed.size(); i++)
      {
        std::string aos_str; 
        std::string func_str; 
        string struct_str;
        bool isParam;
        bool hasPointerElem;

        string AoSType = get<6>(confirmed.at(i));

        string type = get<2>(confirmed.at(i));

        raw_string_ostream aos(aos_str);
        raw_string_ostream func(func_str);
        raw_string_ostream stru(struct_str);
        get<0>(confirmed.at(i))->print(aos);
        errs()<<i+1<<": "<<aos_str;

        Function* funcName = get<1>(confirmed.at(i));
        StructType* structure = get<3>(confirmed.at(i));
        isParam = get<4>(confirmed.at(i));
        hasPointerElem = get<5>(confirmed.at(i));

        if(funcName != NULL)
        {
          funcName->printAsOperand(func);
          errs()<<" - "<<type<<" "<<AoSType<<" used in function: "<<func_str;
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
          errs()<<" - global "<<type<<" "<<AoSType;

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
      errs()<<"\n----------------------- END OF AOS DETECTION -----------------------\n";

      //Set to ::all() if IR is unchanged, otherwise ::none()
      return PreservedAnalyses::none();
    };
};
}