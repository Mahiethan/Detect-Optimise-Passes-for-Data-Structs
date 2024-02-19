//detect dynamic AoS - OLD!!
//get all structs
//create vector of sizes of all structs
//find function calls
  //check if function is malloc
    //get size operand
    //if size is in structSizes
      //dynamic AoS found

/////////////////////////////////////////////

//gloval vector variables
//vector potential
//vector confirmed 
//vector calledFunctions //stores names of functions that where called in either main or in another function - check every called function for a s_AoS or d_AoS

//void detectAoS(AllocaInst* v)

  //get allocatedtype
  //get allocatedtype as a string

  //if it is an arrayallocation (of size greater than 0)
    //check if allocatedtype is a struct
      //static AoS found!
      //store staticAoS in confirmed vector
  //else if string contains 'struct' word inside '[ ]' - e.g. [50 x %struct.node]
    //static AoS found!
    //store staticAoS in confirmed vector
  //endif

//endif

//void checkFunction(Value* v)


/// start with main function

//iterate through main function

  //if v is an AllocaInst
    //detectAoS(v)
  //endif



  



//detect dynamic AoS - requires the arrays to be populated with data to confidently detect them
//iterate through main function until end
  //find malloc DONE
  //get next instruction
    //check if its a store
    //get first operand - a ptr - this is where the array is stored - save this % value in potential vector
//continue iterating through main function
  //checkFunction



  //if CallInst found
    //checkFunction(F,possibleAoSList,confirmedAoSList);
  //find GetElementPtrInst DONE
    //get first operand
    //check if load instruction
      //get first operand
      //if equal to array ptr - % value
        //if return type is struct
          //add AoS to confirmedAoS list
          //remove AoS from possibleAoS list
          //confidently detected a dynamic AoS!!!!!!!!!!!!!!!!!!

//void checkFunction(Function F, vector possibleAoSList, vector confirmedAoSList)
//iterate through function
  //if CallInst found
    //checkFunction(F)
  //find GetElementPtrInst DONE
    //get first operand
    //check if load instruction
      //get first operand
      //if equal to array ptr - % value
        //if return type is struct
          //if return type is struct
          //add AoS to confirmedAoS list
          //remove AoS from possibleAoS list
          //confidently detected a dynamic AoS!!!!!!!!!!!!!!!!!!
  
  //!!do this at the end after collecting all function names
  //iterate through functions if its name is in the vector defined previously
    //find function with name in vector 
      //remove function name from vector
      //get the argument at index (%array) saved in vector, this is the arguement where the array is passed through 
      //iterate through all instructions in function
      //if StoreInst found
        //if first operand == %array
          //get second operand - this is where the malloc-ed array is stored in - save this and now look out for this % value
      //if GetElementPtrInst is found
        //get first operand
    //check if load instruction
      //get first operand
      //if equal to array ptr - % value
        //if return type is struct
          //confidently detected a dynamic AoS!!!!!!!!!!!!!!!!!!

//if list of potential AoS still > 0
  //check if AoS is global dyn_cast<GlobalVariable>
  //look through other functions, (some functions might operate on a global AoS without taking it as an argument)
    //if GetElementPtrInst is found
          //get first operand
      //check if load instruction
        //get first operand
        //if equal to array ptr - % value
          //if return type is struct
            //save function name in a queue
    //iterate through main function
      //check for call inst
        //check if function name matches one in queue
          //confidently detected a dynamic AoS!!!!!!!!!!!!!!!!!! (globally declared)

//CONDITION: for dynamic AoS, the array must be populated with some data, either explicitly in the main function or via a function
//empty dynamic AoS will not be detected 

#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include "./detectAoS.h"

#include <algorithm>
#include <string>
#include <deque>

using namespace llvm;
using namespace std;

//helper functions

// void printAoS(vector<Value*> list)
// {
//   for(int i = 0; i < list.size(); i++)
//   {
//     list.at(i)->print(errs());
//     errs()<<"\n";
//   }
// }

// void removeFromPossibleAoSList(vector<Value*> &list, Value* remove)
// {
//   for (auto it = list.begin(); it != list.end();it++)
//   {
//     if(*it == remove)
//     {
//       list.erase(it);
//       break;
//     }
//   }
// }

// void printFinalAoS(vector<Value*>& list)
// {
//   for(int i = 0; i < list.size(); i++)
//   {
//     list.at(i)->print(errs());
//     errs()<<"\n";
//   }
// }

// bool searchPossibleAoS(vector<Value*>& list, string name, bool addToFinal, vector<Value*>& finalList)
// {
//   for (auto it = list.begin(); it != list.end();it++)
//   {
//     Value* aos = *it;
//     std::string aos_string; 
//     raw_string_ostream aos_stream(aos_string);
//     aos->printAsOperand(aos_stream);
//     if(aos_string.find(name) != std::string::npos)
//     {
//         if(addToFinal == true)
//         {
//           finalList.push_back(aos);
//           list.erase(it);
//         }
//         else
//         {
//           finalList.push_back(aos); //going to popped out soon
//         }
        
//         return true;
//     }

//   }
//   return false;
// }

//get AoS arg

/*
map<Value*, vector<value>> aosArgs
// first = param
// second = vector of AoS that are associated with that param during func call

func getAoSArgs(aos,param,F,index) //initially, this is called in main(), param = aos
  For B in F
    For I in B
      if I is callInst
        get called function
        get Function arg list
        for i in function arg list
          if arglist.at(i) == param
            index = i
            param = F->getArg(i) //get parameter somehow
            if(aosArgs.find(param) != aosArgs.end())
              aosArgs.find(param).second.push_back(aos);
            endif
            else
              vector<Value*> list
              list.push_back(aos)
              aosArgs.insert(make_pair<param,list>)
            endif
            getAoSArgs(aos, param, calledFunction, index)
          endif
        endfor
      endif
  endfor
*/

vector<tuple<Value*,Function*,string,StructType*>> potential;
vector<tuple<Value*,Function*,string,StructType*>> potentialArguments;
vector<tuple<Value*,StoreInst*,string>> argStores;
vector<tuple<Value*,Function*,StructType*>> possibleGlobals;
// vector<tuple<Value*,Function*,string>> confirmed;
vector<tuple<string,vector<int>,Value*,Function*,string>> calledFunction; //stores pair of function name and used argument index of pointer (if any)

Function* originFunction = NULL;

int staticCount = 0;
int dynamicCount = 0;
bool mallocFlag = false;

set<CallInst*> searchedCI;
vector<Function*> searchedFunctionCalls;

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
                // errs()<<"FOUND:";
                // CI->print(errs());
                // errs()<<"\n";
                return make_pair(CI,parentFunction);
              }  
            }
           }
        }
      }
    return result;
  }


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
        // errs()<<"Static Array of Structs (AoS) found"<<"\n"; //static AoS found
        // staticCount++;

        Value* AoS = cast<Instruction>(AI); //store as Value in vector
        StructType* structure = cast<StructType>(returnType);
        // AoS->print(errs());
        potential.push_back(make_tuple(AoS,originFunction,"static",structure));

      //   errs()<<type_str<<"\n";
      }
  }
  else if(type_str.find("struct") != std::string::npos & type_str.find("[") != std::string::npos & type_str.find("]") != std::string::npos) //identifying AoS with size given as int literal - if type contains word "struct"
  {
      if(type_str.find("[0 x") == std::string::npos) //if allocated size is not 0
      {
      //allocation size can be 1 or more
      // errs()<<"Static Array of Structs (AoS) found"<<"\n"; //static AoS found
      // staticCount++;

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
              // AoS->print(errs());
              potential.push_back(make_tuple(AoS,originFunction,"static",structure));
          }
        }
      }
      }
  }
}

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
      // errs()<<"ERasing from potential\n";
      //  errs()<<"Adding back to potential: "<<aos_string<<"\n";
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

StructType* eraseFromPossibleGlobals(Value* val)
{
  for(auto it = possibleGlobals.begin(); it != possibleGlobals.end(); it)
  {
    Value* aos = get<0>(*it);
    StructType* structure = get<2>(*it);
    // std::string aos_string; 
    // raw_string_ostream aos_stream(aos_string);
    // aos->printAsOperand(aos_stream);
    if(aos == val)
    {
      // errs()<<"ERasing from potential\n";
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

void getPotential(Instruction* I) //adding to potential vector
{
  if(auto *SI = dyn_cast<StoreInst>(I))
  {
    Value* operand = SI->getOperand(1); //get second operand of this store instruction
    // potential.push_back(operand);

    
    std::string aos_string; 
    raw_string_ostream aos_stream(aos_string);
    operand->printAsOperand(aos_stream);
    // errs()<<"Potential "<<aos_string<<"\n";

    // if(find(possibleGlobals.begin(),possibleGlobals.end(),operand) != possibleGlobals.end()) //if a malloc uses a global variable that has had a GPE operation applied to it, its is a dynamic AoS
    // {
    //   // errs()<<"Found possible global: "<<aos_string<<"\n";
    //   eraseFromPossibleGlobals(operand);
    //   confirmed.push_back(make_pair(operand,originFunction));
    //   dynamicCount++;
    // }
    // else if(find(potential.begin(),potential.end(),operand) == potential.end())
    // {
    //   eraseFromConfirmed(operand); //remove from confirmed list if it exists before adding it to potential
    //   potential.push_back(make_pair(operand,originFunction));
    // }
    bool foundInPossibleGlobals = false;
    bool foundInPotential = false;
    Value* aos;
    Function* func;
    for(auto it = possibleGlobals.begin(); it != possibleGlobals.end(); it)
    {
      aos = get<0>(*it);
      func = get<1>(*it);
      // std::string aos_string; 
      // raw_string_ostream aos_stream(aos_string);
      // aos->printAsOperand(aos_stream);
      if(aos == operand)
      {
        // errs()<<"ERasing from potential\n";
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
      //errs()<<"Found possible global: "<<aos_string<<"\n";
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
        // std::string aos_string; 
        // raw_string_ostream aos_stream(aos_string);
        // aos->printAsOperand(aos_stream);
        if(aos == operand & func == originFunction)
        {
          // errs()<<"ERasing from potential\n";
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
  mallocFlag = false; //disable flag - don't search for StoreInst after malloc
}

tuple<llvm::Value *, llvm::Function *, std::string, llvm::StructType *> eraseFromPotential(Value* val)
{
  for(auto it = potential.begin(); it != potential.end(); it)
  {
    Value* aos = get<0>(*it);
    StructType* structure = get<3>(*it);
    tuple<Value*,Function*,string,StructType*> elem = *it;
    // std::string aos_string; 
    // raw_string_ostream aos_stream(aos_string);
    // aos->printAsOperand(aos_stream);
    if(aos == val)
    {
      // errs()<<"ERasing from potential\n";
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

void eraseFromPotentialArguments(Value* val)
{
  for(auto it = potentialArguments.begin(); it != potentialArguments.end(); it)
  {
    Value* aos = get<0>(*it);
    StructType* structure = get<3>(*it);
    auto elem = *it;
    // std::string aos_string; 
    // raw_string_ostream aos_stream(aos_string);
    // aos->printAsOperand(aos_stream);
    if(aos == val)
    {
      // errs()<<"ERasing from potential\n";
      potentialArguments.erase(it);
    }
    else
    {
    it++;
    }
  }
}


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

    // errs()<<"removing "<<val_string<<"from uaa "<<aos_string<<"\n";
    if(aos == val)
    {
      // errs()<<"ERasing from usedasArgs\n";
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

Value* gepOperand = NULL;
Value* calledAoS = NULL;
Function* paramFunction = NULL;

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
      // //check if struct is in toFind and set bool
      if(toFind.find(gepStruct) != toFind.end())
        isAoSoA = true;

    }
    if(auto *ST = dyn_cast<StructType>(gep->getSourceElementType()))
    {
      gepStruct = ST;
      //check if struct is in toFind and set bool
      if(toFind.find(gepStruct) != toFind.end())
        isAoSoA = true;
    }

    // errs()<<"Checking operand: "<<op_string<<"\n";

    while(isa<LoadInst>(operand) == false & isa<AllocaInst>(operand) == false & isa<GlobalVariable>(operand) == false)
    {
      if(auto *GEP = dyn_cast<GetElementPtrInst>(operand))
      {
        operand = GEP->getOperand(0);
        // operand->printAsOperand(ops);
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

      // errs()<<"does it equal "<<ptr_string<<"\n";

      if(op_string == ptr_string) //if loaded operand is one of the malloc-ed arrays detected earlier
      {
        // errs()<<"Dynamic Array of Structs (AoS) found"<<"\n"; //dynamic AoS found
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
        // errs()<<"Size of argStores: "<<argStores.size()<<"\n";
        // errs()<<"Size of potential: "<<potential.size()<<"\n";
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

        // errs()<<"for operand: "<<op_string<<"\n";
      

      std::string ptr_string; 
      raw_string_ostream ptr(ptr_string);
      aos->print(ptr);

      // errs()<<"does it equal "<<ptr_string<<"\n";

      if(op_string == ptr_string) //if loaded operand is one of the malloc-ed arrays detected earlier
      {
        // errs()<<"Dynamic Array of Structs (AoS) found"<<"\n"; //dynamic AoS found
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
      
        // errs()<<"Size of argStores: "<<argStores.size()<<"\n";
        // errs()<<"Size of potential: "<<potential.size()<<"\n";
        return true;
      }
      gepOperand = operand;

    }
  
  return false;
}

void getCalledFunctions(CallInst* CI, Function* orig)
{
  vector<int> indices;
  Value* aos;
  Value* paramAoS;
  string type = "undefined";
  int index = 0;
  // CI->print(errs());
  // errs()<<"\n";
  CI->setTailCall(false);
  // CI->print(errs());
  // errs()<<"\n";
  Function* f = CI->getCalledFunction();
  string funcName = f->getName().str();
  // errs()<<funcName<<"\n";
  // errs()<<"Checking func call "<<funcName<<"\n";
  for(auto a = CI->arg_begin(); a != CI->arg_end(); a++) //get all function arguments
  {
    bool detect = false;
    if(detect == true)
    {
      break;
    }

    Value* argument = a->get(); //get argument o
    Value* operand;
    if(auto LI = dyn_cast<LoadInst>(argument))
      operand = LI->getOperand(0); //get first operand (value being loaded)      
    else
      operand = argument;

    //convert to string for comparison
    std::string op_string; 
    raw_string_ostream ops(op_string);
    operand->printAsOperand(ops);

      // bool detect = false;
      
      for(int i = 0; i < potential.size(); i++)
      {
        aos = get<0>(potential.at(i));
        std::string value_string; 
        raw_string_ostream aos_stream(value_string);
        aos->printAsOperand(aos_stream);
        // errs()<<"pointer: "<<value_string<<"\n";
        // errs()<<"operand: "<<op_string<<"\n";
        if(value_string == op_string)
        {
          // errs()<<"adding "<<value_string<<" to indices\n";
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
          // errs()<<"pointer: "<<value_string<<"\n";
          // errs()<<"operand: "<<op_string<<"\n";
          if(value_string == op_string)
          {
            // errs()<<"adding to indices\n";
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
            possibleGlobals.push_back(make_tuple(operand,f,nullptr)); //@globalFive in dynamic_AoS.c called in main() function 
          detect = true;
        }
      }
    
    index++;
  }
  // errs()<<"Size of indices: "<<indices.size()<<"\n";
  // errs()<<"Adding function: "<<funcName<<"with indices size"<<indices.size()<<"that operates on"<<valName<<"\n";
  calledFunction.push_back(std::make_tuple(funcName,indices,paramAoS,orig,type));
}

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
          
          // errs()<<"Get operand "<<op_string<<"\n";

          for(int i = 0; i < arguments.size(); i++)
          {
            std::string arg_string; 
            raw_string_ostream args(arg_string);
            arguments.at(i)->printAsOperand(args);
            // errs()<<"Size "<<i<<" Search for "<<arg_string<<"\n";
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

namespace {

struct detectAoS : public PassInfoMixin<detectAoS> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {

        errs()<<"\n-------------------------- AOS DETECTION --------------------------\n\n";

        detectAoSCalled = true;

        // errs()<<"AoSoA list size: "<<AoSoAList.size()<<"\n";

        // for(auto it = AoSoAList.begin(); it != AoSoAList.end(); it++)
        // {
        //   auto elem = *it;
        //   potential.push_back(make_tuple(get<0>(elem),get<1>(elem),get<2>(elem),get<3>(elem)));
        //   errs()<<"Adding to potential\n";
        // }

        vector<Value*> possibleAoSList; //vector which stores pointer locations of possible AoS - these need to be inspected and confirmed
        vector<Value*> AoS_Values; //vector which stores pointer locations of possible AoS - these need to be inspected and confirmed
        deque<tuple<string, int, Value*>> functionInspectionList; //vector that stores functions that need to be inspected for GPE instructions in order to fully determine that a dynamic AoS exists

        // errs()<<"Getting globals\n";
        for(GlobalVariable& gv : M.globals())
        {
          // if((gv.getLinkage() == GlobalValue::LinkageTypes::PrivateLinkage) & (gv.getType() == Type::getInt1Ty(split_Context)) & ())
          if(gv.getName() == "permitStructSplittingFlag")
            permitStructSplitting = false;
          else
            permitStructSplitting = true;

           std::string type_str; 
           raw_string_ostream to_str(type_str);
           gv.print(to_str);
           
          if(type_str.find("struct") != std::string::npos & type_str.find("[") != std::string::npos & type_str.find("]") != std::string::npos) //identifying AoS with size given as int literal - if type contains word "struct"
          {
            if(type_str.find("[0 x") == std::string::npos) //if allocated size is not 0
            {
              //allocation size can be 1 or more
              // errs()<<"Static Array of Structs (AoS) found"<<"\n"; //static AoS found
              // staticCount++;
              Constant* constValue; 
              constValue = gv.getInitializer(); //get the initializer

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
                // AoS->printAsOperand(errs());
                potential.push_back(make_tuple(AoS,originFunction,"static",ST));
              }
            }
          }
        }
        
        bool checkMain = false;
        for (auto &F : M) //iterate through all functions in the Module and print their names
        { 
            if(checkMain == true)
              break;
            // errs()<<"In function: "<<F.getName()<<"\n";
            if(F.getName().find("main") != std::string::npos)
            {
              originFunction = &F;
              for (auto &B : F)  //iterate through all blocks in each function
              { 
                  for (auto &I : B) //iterate through all instructions in each block
                  { 
                    if(mallocFlag == true)
                      getPotential(&I);
                    else if(auto *SI = dyn_cast<StoreInst>(&I)) //find global static AoS used in a GEP operation
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

                      // errs()<<op_str<<"\n";

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

                      if(found == false)
                      {
                        for(auto it = potential.begin(); it != potential.end(); it++)
                        {
                          Value* aos = get<0>(*it);
                          string type = get<2>(*it);
                          StructType* structure = get<3>(*it);

                          if(loadedAoS == aos) //an potential AoS could be stored into an empty pointer - add this pointer to 'potential' list
                          {
                            eraseFromPossibleGlobals(SI->getOperand(1));
                            potential.push_back(make_tuple(SI->getOperand(1),originFunction,type,structure));
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
                      // CI->print(errs());
                      // errs()<<"\n";
                      CI->setTailCall(false);
                      // CI->print(errs());
                      // errs()<<"\n";
                      Function* f = CI->getCalledFunction();
                      string funcName = f->getName().str();
                      // errs()<<funcName<<"\n";
                      if(funcName == "malloc" | funcName == "calloc")
                      {
                        // mallocAttributes = f->getAttributes();
                        mallocFlag = true;
                      }
                      else if(funcName != "free")
                        getCalledFunctions(CI,&F);
                    }
                    else if(auto *GEP = dyn_cast<GetElementPtrInst>(&I))
                    {
                      bool found = false;
                      Value* aos;
                      string type;
                      if(GEP->getResultElementType()->isStructTy())
                      {
                        for(auto it = potential.begin(); it != potential.end(); it++)
                        {
                          aos = get<0>(*it);
                          type = get<2>(*it);
                          std::string ptr_string; 
                          raw_string_ostream ptr(ptr_string);
                          aos->printAsOperand(ptr);
                          // errs()<<"Searching for "<<ptr_string<<"\n";
                          if(checkGEP(GEP,aos,false,type) == true)
                          {
                            // errs()<<"Found "<<ptr_string<<"\n";
                            found = true;
                            eraseFromPotential(aos);
                            break;
                          }
                        }
                        // errs()<<found<<"\n";
                        if(found == false)
                        {
                          // errs()<<"adding to possibleGlobals\n";
                          if(gepOperand != NULL)
                          {
                            if(auto *GV = dyn_cast<GlobalVariable>(gepOperand))
                            {
                              StructType* structure = cast<StructType>(GEP->getResultElementType());
                              bool inConfirmed = findInConfirmed(gepOperand);
                              if(inConfirmed == false)
                                possibleGlobals.push_back(make_tuple(gepOperand,originFunction,structure)); //@globalFive in dynamic_AoS.c called in main() function 
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

        // errs()<<"Size of calledFunctions vector: "<<calledFunction.size()<<"\n";
        // for(int i = 0; i < calledFunction.size(); i++)
        // {
        //   vector<int> indices = get<1>(calledFunction.at(i));
        //   // errs()<<get<0>(calledFunction.at(i))<<" uses ";
        //   for(int j = 0; j < indices.size(); j++)
        //   {
        //     // errs()<<"index "<<indices.at(j)<<"\n";
        //   }
        // }

        // errs()<<potential.size()<<"\n";
        // errs()<<confirmed.size()<<"\n";

      for(auto it = calledFunction.begin(); it != calledFunction.end(); it)
      {
        tuple<string,vector<int>,Value*,Function*,string> searchFunc = *it;
        string type = get<4>(searchFunc);
        calledFunction.erase(it);
        vector<int> argIndices = get<1>(searchFunc);
        calledAoS = get<2>(searchFunc); //get AoS that was used as parameter in function call
        paramFunction = get<3>(searchFunc);
        for (auto &F : M) //iterate through all functions in the Module and print their names
        { 
          if(F.getName() == get<0>(searchFunc))
          {
            originFunction = &F;
            // errs()<<"Searching "<<get<0>(searchFunc)<<"\n";
            if(argIndices.size() != 0)
            {
              argStores = getArgumentStores(&F,argIndices,type);
              // errs()<<"compare indices "<<argIndices.size()<<" to argStores "<<argStores.size()<<"\n";
            }

            for(auto &B : F)
            {
              for(auto &I : B)
              {
                if(mallocFlag == true)
                  getPotential(&I);
                else if(auto *SI = dyn_cast<StoreInst>(&I)) //find global static AoS used in a GEP operation
                {
                  Value* operand = SI->getOperand(1);
                  std::string op_str; 
                  raw_string_ostream to_str(op_str);
                  operand->printAsOperand(to_str);

                  // errs()<<op_str<<"\n";

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
                      break;
                    }
                  }

                }
                else if(auto *AI = dyn_cast<AllocaInst>(&I))
                  detectStaticAoS(AI);
                else if(auto *CI = dyn_cast<CallInst>(&I))
                {
                  // CI->print(errs());
                  // errs()<<"\n";
                  CI->setTailCall(false);
                  // CI->print(errs());
                  // errs()<<"\n";
                  Function* f = CI->getCalledFunction();
                  string funcName = f->getName().str();
                  // errs()<<funcName<<"\n";
                  if(funcName == "malloc" | funcName == "calloc")
                    mallocFlag = true;
                  else if(funcName != "free")
                  {
                    getCalledFunctions(CI,&F);
                  }
                }
                else if(auto *GEP = dyn_cast<GetElementPtrInst>(&I))
                {
                  // GEP->print(errs());
                  // errs()<<"\n";
                  if(GEP->getResultElementType()->isStructTy())
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
                          StructType* structure = cast<StructType>(GEP->getResultElementType());
                          bool inConfirmed = findInConfirmed(gepOperand);
                          if(inConfirmed == false)
                            possibleGlobals.push_back(make_tuple(gepOperand,originFunction,structure)); //@globalFive in dynamic_AoS.c called in main() function 
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        //remove duplicate function calls - store value as well
        // for(auto it = calledFunction.begin(); it != calledFunction.end(); it)
        // {
        //   tuple<string,vector<int>,string> val = *it;
        //   if((get<0>(val) == get<0>(searchFunc)) & (get<1>(val) == get<1>(searchFunc)) & (get<2>(val) == get<2>(searchFunc)))
        //   {
        //     errs()<<"Removing function: "<<get<0>(val)<<" with val:"<<get<2>(val)<<" and"<<get<2>(searchFunc)<<"\n";
        //     calledFunction.erase(it);
        //   }
        //   else
        //   {
        //     it++;
        //   }
        // }
        }
      }

      //correctly detect AoS used as function arguments

      /*

        pair<value,function> getOrigin(F,aos,M)
        {
          result = pair<F,aos>
          search thru all F,B,I
            if callInst found
              if call to F found & not found in searchedCI vector
                  searchedCIvector.append(call)
                  return pair<value,function> res = getOrigin(new F)
          end
             
          return result
        }

        for each aos in confirmed
          current confirmed value = temp;
          result;
          newAoS = current aos;
          newFunc = nullptr;
          if aos = a callInsts
            get the associated function F
            while(newAoS != callInst & newFunc == nullptr)
              pair<Value*,Function*> result = look for a function call to this function: getOrigin(F,aos,M)
              for each operand used in the function call
                check if the operand exists in potential and if function matches
                  if yes, delete from potential
                    newAOS = aos
              endfor
              newFunc = origin function
            endwhile

          update temp with new values from newValues
        endfor
      
      */

     

     for(int a = 0; a < confirmed.size(); a++)
     {
      // errs()<<"AOS: "<<a<<"\n";
       auto currConfirmed = confirmed.at(a);
       Value* aos = get<0>(currConfirmed);
       Function* func = get<1>(currConfirmed);
       pair<Value*,Function*> result;
       bool found = false;
       string type = "dynamic";
       

      if(auto *SI = dyn_cast<StoreInst>(aos))
      {
        //  errs()<<"To search call: "<<func->getName()<<"\n";
        searchedFunctionCalls.push_back(func);
        while(found == false)
        {
          result = getOrigin(func,aos,&M);
          // errs()<<"------------RESULTS----------------\n";
          // result.first->print(errs());
          // errs()<<"\n";
          // result.second->printAsOperand(errs());
          // errs()<<"\n";
          // errs()<<"-----------------------------------\n";

          if(auto *SI = dyn_cast<StoreInst>(result.first)) //no callinst found
          {
            searchedFunctionCalls.pop_back();
            func = searchedFunctionCalls.at(searchedFunctionCalls.size() - 1);
            // errs()<<"Checking "<<func->getName()<<" again\n";
          }
          else if(auto *CI = dyn_cast<CallInst>(result.first))
          {
            for(auto a = CI->arg_begin(); a != CI->arg_end(); a++)
            {
              Value* toFind = *a;
              if(auto *LI = dyn_cast<LoadInst>(toFind))
                toFind = LI->getOperand(0);

              // errs()<<"Call inst operand: ";
              // toFind->print(errs());
              // errs()<<"\n";

              for(auto it = potentialArguments.begin(); it != potentialArguments.end(); it)
              {
                Value* aos = get<0>(*it);
                Function* func = get<1>(*it);

                // errs()<<"Is it: ";
                // aos->print(errs());
                // errs()<<"\n";

                if(aos == toFind)
                {
                  // errs()<<"FOUND AOS\n";
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

                  // errs()<<"Is it: ";
                  // aos->print(errs());
                  // errs()<<"\n";

                  if(aos == toFind)
                  {
                    // errs()<<"FOUND AOS\n";
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
                // searchedCI.clear();
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

      int staticCountTemp = 0;
      int dynamicCountTemp = 0;

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

      vector<pair<StructType*, bool>> checkedStructs;

      //iterate through all structs in the confirmed list, and determine whether it contains a ptr field - which could make it recursive
      for(int i = 0; i < confirmed.size(); i++)
      {
        // if(get<5>(confirmed.at(i)) == true)
        //   continue;

        StructType* structure = get<3>(confirmed.at(i));

        /// for each used structure, store the size of each struct

        DataLayout* TD = new DataLayout(&M); //to get size and layout of structs

        // origStructSizes.insert(make_pair(structure,TD->getStructLayout(structure)->getSizeInBytes()));
        origStructSizes.insert(make_pair(structure,make_pair(TD->getTypeAllocSize(structure),TD->getTypeAllocSize(structure))));


        bool alreadyChecked = false;

        for(int j = 0; j < checkedStructs.size(); j++)
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
            get<5>(confirmed.at(i)) = true;
            break;
          }
        }
      }

        errs()<<"Size of potential list: "<<potential.size()<<"\n";
        for(int i = 0; i < potential.size(); i++)
        {
          std::string aos_str; 
          std::string func_str; 
          string struct_str;
          string type;
          // string type = get<2>(potential.at(i));
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
            errs()<<" - global "<<type<<" "<<AoSType<<"\n";
          }
          errs()<<"\n";
        }
        errs()<<"\n----------------------- END OF AOS DETECTION -----------------------\n";
        //Set to ::all() if IR is unchanged, otherwise ::none()
        return PreservedAnalyses::none();
    };
};
}


// //Creates plugin for pass - required
// extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
// llvmGetPassPluginInfo() {
//   return {
//     LLVM_PLUGIN_API_VERSION, "detectAoS", "v0.1",
//     [](PassBuilder &PB) {
//       PB.registerPipelineParsingCallback(
//         [](StringRef Name, ModulePassManager &MPM, //Module pass
//         ArrayRef<PassBuilder::PipelineElement>) {
//           if(Name == "detectAoS"){ //name of pass
//             MPM.addPass(detectAoS());
//             return true;
//           }
//           return false;
//         }
//       );
//     }
//   };
// }