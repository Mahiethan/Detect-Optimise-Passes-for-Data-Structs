//detect dynamic SoA - OLD!!
//get all structs
//create vector of sizes of all structs
//find function calls
  //check if function is malloc
    //get size operand
    //if size is in structSizes
      //dynamic SoA found

/////////////////////////////////////////////

//gloval vector variables
//vector potentialSoA
//vector confirmedSoA 
//vector calledFunctions //stores names of functions that where called in either main or in another function - check every called function for a s_AoS or d_AoS

//void detectAoS(AllocaInst* v)

  //get allocatedtype
  //get allocatedtype as a string

  //if it is an arrayallocation (of size greater than 0)
    //check if allocatedtype is a struct
      //static SoA found!
      //store staticAoS in confirmedSoA vector
  //else if string contains 'struct' word inside '[ ]' - e.g. [50 x %struct.node]
    //static SoA found!
    //store staticAoS in confirmedSoA vector
  //endif

//endif

//void checkFunction(Value* v)


/// start with main function

//iterate through main function

  //if v is an AllocaInst
    //detectAoS(v)
  //endif



  



//detect dynamic SoA - requires the arrays to be populated with data to confidently detect them
//iterate through main function until end
  //find malloc DONE
  //get next instruction
    //check if its a store
    //get first operand - a ptr - this is where the array is stored - save this % value in potentialSoA vector
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
          //add SoA to confirmedAoS list
          //remove SoA from possibleAoS list
          //confidently detected a dynamic SoA!!!!!!!!!!!!!!!!!!

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
          //add SoA to confirmedAoS list
          //remove SoA from possibleAoS list
          //confidently detected a dynamic SoA!!!!!!!!!!!!!!!!!!
  
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
          //confidently detected a dynamic SoA!!!!!!!!!!!!!!!!!!

//if list of potentialSoA SoA still > 0
  //check if SoA is global dyn_cast<GlobalVariable>
  //look through other functions, (some functions might operate on a global SoA without taking it as an argument)
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
          //confidently detected a dynamic SoA!!!!!!!!!!!!!!!!!! (globally declared)

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

//get SoA arg

/*
map<Value*, vector<value>> aosArgs
// first = param
// second = vector of SoA that are associated with that param during func call

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
            getAoSArgs(aos, param, calledFunctionSoA, index)
          endif
        endfor
      endif
  endfor
*/
vector<tuple<Value*,Function*,string,StructType*>> potentialSoA;
vector<tuple<Value*,Function*,string,StructType*>> potentialArgumentsSoA;
vector<tuple<Value*,StoreInst*,string>> argStoresSoA;
vector<tuple<Value*,Function*,StructType*>> possibleGlobalsSoA;
// vector<tuple<Value*,Function*,string>> confirmedSoA;
set<tuple<int,string,vector<int>,Value*,Function*,string>> calledFunctionSoA; //stores pair of function name and used argument index of pointer (if any)
int calledNumSoA = 0;

map<string,set<string>> functionStreamSoA;

 
Function* originFunctionSoA = NULL;

int nonPointerCount = 0;
int pointerCount = 0;
bool mallocFlagSoA = false;

set<CallInst*> searchedCISoA;
vector<Function*> searchedFunctionCallsSoA;

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
        // errs()<<"Static Array of Structs (SoA) found"<<"\n"; //static SoA found
        // nonPointerCount++;

        Value* SoA = cast<Instruction>(AI); //store as Value in vector
        StructType* structure = cast<StructType>(returnType);
        // SoA->print(errs());
        potentialSoA.push_back(make_tuple(SoA,originFunctionSoA,"static",structure));

      //   errs()<<type_str<<"\n";
      }
  }
  else //AoSoA
  {
    if(toFind.find(returnType) != toFind.end()) //if the allocated type is a struct
      {
        // errs()<<"Static Array of Structs (SoA) found"<<"\n"; //static SoA found
        // nonPointerCount++;

        Value* SoA = cast<Instruction>(AI); //store as Value in vector
        StructType* structure = cast<StructType>(returnType);
        // SoA->print(errs());
        AoSoAList.push_back(make_tuple(SoA,originFunctionSoA,"static",structure));

      //   errs()<<type_str<<"\n";
      }
  }
  // else if(type_str.find("struct") != std::string::npos & type_str.find("[") != std::string::npos & type_str.find("]") != std::string::npos) //identifying SoA with size given as int literal - if type contains word "struct"
  // {
  //     if(type_str.find("[0 x") == std::string::npos) //if allocated size is not 0
  //     {
  //     //allocation size can be 1 or more
  //     // errs()<<"Static Array of Structs (SoA) found"<<"\n"; //static SoA found
  //     // nonPointerCount++;

  //     if(auto *AT = dyn_cast<ArrayType>(AI->getAllocatedType()))
  //     {
  //       Type* elem = AT->getArrayElementType();
  //       int num = AT->getArrayNumElements();
  //       if(num > 0)
  //       {
  //         if(auto *ST = dyn_cast<StructType>(elem))
  //         {
  //             Value* SoA = cast<Instruction>(AI); //store as Value in vector
  //             StructType* structure = cast<StructType>(elem);
  //             // SoA->print(errs());
  //             potentialSoA.push_back(make_tuple(SoA,originFunctionSoA,"static",structure));
  //         }
  //       }
  //     }
  //     }
  // }
}

bool findInConfirmedSoA(Value* val)
{
  for(auto it = confirmedSoA.begin(); it != confirmedSoA.end(); it)
  {
    Value* aos = get<0>(*it);
  
    if(aos == val)
    {
      printf("Found in confirmedSoA\n");
      return true;
    }
    else
      it++;
  }
  return false;
}

StructType* eraseFromConfirmedSoA(Value* val)
{
  for(auto it = confirmedSoA.begin(); it != confirmedSoA.end(); it)
  {
    Value* aos = get<0>(*it);
    string type = get<2>(*it);
    StructType* structure = get<3>(*it);
    std::string aos_string; 
    raw_string_ostream aos_stream(aos_string);
    aos->printAsOperand(aos_stream);
    if(aos == val)
    {
      // errs()<<"ERasing from potentialSoA\n";
      //  errs()<<"Adding back to potentialSoA: "<<aos_string<<"\n";
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

StructType* eraseFromPossibleGlobalsSoA(Value* val)
{
  for(auto it = possibleGlobalsSoA.begin(); it != possibleGlobalsSoA.end(); it)
  {
    Value* aos = get<0>(*it);
    StructType* structure = get<2>(*it);
    // std::string aos_string; 
    // raw_string_ostream aos_stream(aos_string);
    // aos->printAsOperand(aos_stream);
    if(aos == val)
    {
      // errs()<<"ERasing from potentialSoA\n";
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
void getPotentialSoA(Instruction* I) //adding to potentialSoA vector
{
  if(auto *SI = dyn_cast<StoreInst>(I))
  {
    Value* operand = SI->getOperand(1); //get second operand of this store instruction
    // potentialSoA.push_back(operand);

    
    std::string aos_string; 
    raw_string_ostream aos_stream(aos_string);
    operand->printAsOperand(aos_stream);
    // errs()<<"Potential "<<aos_string<<"\n";

    // if(find(possibleGlobalsSoA.begin(),possibleGlobalsSoA.end(),operand) != possibleGlobalsSoA.end()) //if a malloc uses a global variable that has had a GPE operation applied to it, its is a dynamic SoA
    // {
    //   // errs()<<"Found possible global: "<<aos_string<<"\n";
    //   eraseFromPossibleGlobalsSoA(operand);
    //   confirmedSoA.push_back(make_pair(operand,originFunctionSoA));
    //   pointerCount++;
    // }
    // else if(find(potentialSoA.begin(),potentialSoA.end(),operand) == potentialSoA.end())
    // {
    //   eraseFromConfirmedSoA(operand); //remove from confirmedSoA list if it exists before adding it to potentialSoA
    //   potentialSoA.push_back(make_pair(operand,originFunctionSoA));
    // }
    bool foundInPossibleGlobals = false;
    bool foundInPotential = false;
    Value* aos;
    Function* func;
    for(auto it = possibleGlobalsSoA.begin(); it != possibleGlobalsSoA.end(); it)
    {
      aos = get<0>(*it);
      func = get<1>(*it);
      // std::string aos_string; 
      // raw_string_ostream aos_stream(aos_string);
      // aos->printAsOperand(aos_stream);
      if(aos == operand)
      {
        // errs()<<"ERasing from potentialSoA\n";
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
      StructType* structure = eraseFromPossibleGlobalsSoA(operand);
      if(!findInConfirmedSoA(operand))
      {
        confirmedSoA.push_back(make_tuple(aos,func,"dynamic",structure,false,false));
        pointerCount++;
      }

      
    }
    else
    {
      for(auto it = potentialSoA.begin(); it != potentialSoA.end(); it)
      {
        aos = get<0>(*it);
        func = get<1>(*it);
        // std::string aos_string; 
        // raw_string_ostream aos_stream(aos_string);
        // aos->printAsOperand(aos_stream);
        if(aos == operand & func == originFunctionSoA)
        {
          // errs()<<"ERasing from potentialSoA\n";
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

tuple<llvm::Value *, llvm::Function *, std::string, llvm::StructType *> eraseFromPotentialSoA(Value* val)
{
  for(auto it = potentialSoA.begin(); it != potentialSoA.end(); it)
  {
    Value* aos = get<0>(*it);
    StructType* structure = get<3>(*it);
    tuple<Value*,Function*,string,StructType*> elem = *it;
    // std::string aos_string; 
    // raw_string_ostream aos_stream(aos_string);
    // aos->printAsOperand(aos_stream);
    if(aos == val)
    {
      // errs()<<"ERasing from potentialSoA\n";
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

void eraseFromPotentialArgumentsSoA(Value* val)
{
  for(auto it = potentialArgumentsSoA.begin(); it != potentialArgumentsSoA.end(); it)
  {
    Value* aos = get<0>(*it);
    StructType* structure = get<3>(*it);
    auto elem = *it;
    // std::string aos_string; 
    // raw_string_ostream aos_stream(aos_string);
    // aos->printAsOperand(aos_stream);
    if(aos == val)
    {
      // errs()<<"ERasing from potentialSoA\n";
      potentialArgumentsSoA.erase(it);
    }
    else
    {
    it++;
    }
  }
}

pair<StoreInst*,string> eraseFromArgStoresSoA(Value* val)
{
  for(auto it = argStoresSoA.begin(); it != argStoresSoA.end(); it)
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

bool checkGEPSoA(GetElementPtrInst *gep, Value* aos, bool isParam, string type)
{
    Value* operand = gep->getOperand(0);
    StructType* gepStruct = nullptr;

    //getting operand as string
    std::string op_string; 
    raw_string_ostream ops(op_string);
    operand->print(ops);

    if(auto *ST = dyn_cast<StructType>(gep->getSourceElementType()))
    {
      // if(gep->getResultElementType()->isArrayTy())
        gepStruct = ST;
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
    
    if(auto *GV = dyn_cast<GlobalVariable>(operand)) //GEP operating on a global SoA
    {
      std::string ptr_string; 
      raw_string_ostream ptr(ptr_string);
      aos->print(ptr);

      if(op_string == ptr_string)
      {
        if(type == "static")
          nonPointerCount++;
        else if(type == "dynamic")
          pointerCount++;

        // output not needed
        StructType* structure = get<3>(eraseFromPotentialSoA(aos));
        if(structure == nullptr)
          structure = eraseFromPossibleGlobalsSoA(aos);
        confirmedSoA.push_back(make_tuple(aos,originFunctionSoA,type,gepStruct,false,false));
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
        // errs()<<"Dynamic Array of Structs (SoA) found"<<"\n"; //dynamic SoA found
        if(type == "static")
          nonPointerCount++;
        else if(type == "dynamic")
          pointerCount++;

        // output not needed
        StructType* structure =  get<3>(eraseFromPotentialSoA(aos));
        if(structure == nullptr)
          structure = eraseFromPossibleGlobalsSoA(aos);

        if(isParam)
        {
          Value* storedAoS = eraseFromArgStoresSoA(aos).first;
          confirmedSoA.push_back(make_tuple(storedAoS,originFunctionSoA,type,gepStruct,true,false));
        }
        else
          confirmedSoA.push_back(make_tuple(aos,originFunctionSoA,type,gepStruct,false,false));
        // errs()<<"Size of argStoresSoA: "<<argStoresSoA.size()<<"\n";
        // errs()<<"Size of potentialSoA: "<<potentialSoA.size()<<"\n";
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

        // errs()<<"for operand: "<<op_string<<"\n";
      

      std::string ptr_string; 
      raw_string_ostream ptr(ptr_string);
      aos->print(ptr);

      // errs()<<"does it equal "<<ptr_string<<"\n";

      if(op_string == ptr_string) //if loaded operand is one of the malloc-ed arrays detected earlier
      {
        // errs()<<"Dynamic Array of Structs (SoA) found"<<"\n"; //dynamic SoA found
       if(type == "static")
          nonPointerCount++;
        else if(type == "dynamic")
          pointerCount++;

        // output not needed
        StructType* structure =  get<3>(eraseFromPotentialSoA(aos));
        if(structure == nullptr)
          structure = eraseFromPossibleGlobalsSoA(aos);

        if(isParam)
        {
          Value* storedAoS = eraseFromArgStoresSoA(aos).first;
          confirmedSoA.push_back(make_tuple(storedAoS,originFunctionSoA,type,gepStruct,true,false));
        }
        else
          confirmedSoA.push_back(make_tuple(aos,originFunctionSoA,type,gepStruct,false,false));
      
        // errs()<<"Size of argStoresSoA: "<<argStoresSoA.size()<<"\n";
        // errs()<<"Size of potentialSoA: "<<potentialSoA.size()<<"\n";
        return true;
      }
      gepOperandSoA = operand;

    }
  
  return false;
}

void getCalledFunctionsSoA(CallInst* CI, Function* orig)
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

    // errs()<<op_string<<"\n";

      // bool detect = false;
      
      for(int i = 0; i < potentialSoA.size(); i++)
      {
        aos = get<0>(potentialSoA.at(i));
        std::string value_string; 
        raw_string_ostream aos_stream(value_string);
        aos->printAsOperand(aos_stream);
        // errs()<<"pointer: "<<value_string<<"\n";
        // errs()<<"operand: "<<op_string<<"\n";
        if(value_string == op_string)
        {
          // errs()<<"adding "<<value_string<<" to indices\n";
          indices.push_back(index);
          //add to new list: potentialArgumentsSoA
          tuple<Value*,Function*,string,StructType*> elem = eraseFromPotentialSoA(aos);
          type = get<2>(elem);
          potentialArgumentsSoA.push_back(elem);
          detect = true;
          paramAoS = aos;

          break;
        }
      }
      if(detect == false)
      {
        for(int i = 0; i < argStoresSoA.size(); i++)
        {
          aos = get<0>(argStoresSoA.at(i));
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
            eraseFromArgStoresSoA(aos);
            paramAoS = aos;

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
            possibleGlobalsSoA.push_back(make_tuple(operand,f,nullptr)); //@globalFive in dynamic_AoS.c called in main() function 
          detect = true;
        }
      }
    
    index++;
  }
  // errs()<<"Size of indices: "<<indices.size()<<"\n";
  // errs()<<"Adding function: "<<funcName<<"with indices size"<<indices.size()<<"that operates on"<<valName<<"\n";
  calledFunctionSoA.insert(std::make_tuple(calledNumSoA,funcName,indices,paramAoS,orig,type));
  calledNumSoA++;
}

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
        //add this struct to "toFind" vector

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
            // get type of field i
            Type* fieldType = currStruct->getElementType(i);

            if(fieldType->isArrayTy())
            {
              // errs()<<"Aggregate type found\n";

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

              toFind.insert(make_pair(currStruct,size));

          }

        }

        // errs()<<"\n\n";

        // errs()<<"Getting globals\n";
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
              // errs()<<"Adding AoSoA ";
              // Global->print(errs());
              // errs()<<" to confirmedSoA list\n";
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
              // errs()<<"Adding SoA ";
              // Global->print(errs());
              // errs()<<" to confirmedSoA list\n";
              potentialSoA.push_back(make_tuple(SoA,originFunctionSoA,"static",ST));
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

                      // errs()<<op_str<<"\n";

                      for(auto it = potentialSoA.begin(); it != potentialSoA.end(); it++)
                      {
                        Value* aos = get<0>(*it);
                        std::string ptr_string; 
                        raw_string_ostream ptr(ptr_string);
                        aos->printAsOperand(ptr);

                        if((op_str.find("getelementptr") != std::string::npos) & (op_str.find(ptr_string) != std::string::npos))
                        {
                          StructType* structure =  get<3>(eraseFromPotentialSoA(aos));
                          nonPointerCount++;
                          confirmedSoA.push_back(make_tuple(aos,originFunctionSoA,"static",structure,false,false));
                          found = true;
                          break;
                        }
                      }

                      if(found == false)
                      {
                        for(auto it = potentialSoA.begin(); it != potentialSoA.end(); it++)
                        {
                          Value* aos = get<0>(*it);
                          string type = get<2>(*it);
                          StructType* structure = get<3>(*it);

                          if(loadedAoS == aos) //an potentialSoA SoA could be stored into an empty pointer - add this pointer to 'potentialSoA' list
                          {
                            eraseFromPossibleGlobalsSoA(SI->getOperand(1));
                            potentialSoA.push_back(make_tuple(SI->getOperand(1),originFunctionSoA,type,structure));
                            found = true;
                            break;
                          }
                        }

                        for(auto it = confirmedSoA.begin(); it != confirmedSoA.end(); it++)
                        {
                          Value* aos = get<0>(*it);
                          string type = get<2>(*it);
                          StructType* structure = get<3>(*it);

                          if(loadedAoS == aos) //an potential AoS could be stored into an empty pointer - add this pointer to 'potential' list
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
                                errs()<<it->second<<" dviide by "<<constIntValue<<"\n";
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
                              // errs()<<"Size :"<<constIntValue<<"\n";

                              // StructType* SoA = nullptr;

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
                        getCalledFunctionsSoA(CI,&F);
                      }
                    }
                    else if(auto *GEP = dyn_cast<GetElementPtrInst>(&I))
                    {
                      bool found = false;
                      Value* aos;
                      string type;
                      if((toFind.find(GEP->getSourceElementType()) != toFind.end()))
                      {
                        for(auto it = potentialSoA.begin(); it != potentialSoA.end(); it++)
                        {
                          aos = get<0>(*it);
                          type = get<2>(*it);
                          std::string ptr_string; 
                          raw_string_ostream ptr(ptr_string);
                          aos->printAsOperand(ptr);
                          // errs()<<"Searching for "<<ptr_string<<"\n";
                          if(checkGEPSoA(GEP,aos,false,type) == true)
                          {
                            // errs()<<"Found "<<ptr_string<<"\n";
                            found = true;
                            eraseFromPotentialSoA(aos);
                            break;
                          }
                        }
                        // errs()<<found<<"\n";
                        if(found == false)
                        {
                          // errs()<<"adding to possibleGlobalsSoA\n";
                          if(gepOperandSoA != NULL)
                          {
                            if(auto *GV = dyn_cast<GlobalVariable>(gepOperandSoA))
                            {
                              StructType* structure = cast<StructType>(GEP->getSourceElementType());
                              bool inConfirmed = findInConfirmedSoA(gepOperandSoA);
                              if(inConfirmed == false & (findInAoSoAList(gepOperandSoA) == false))
                                possibleGlobalsSoA.push_back(make_tuple(gepOperandSoA,originFunctionSoA,structure)); //@globalFive in dynamic_AoS.c called in main() function 
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

        // errs()<<"Size of calledFunctions vector: "<<calledFunctionSoA.size()<<"\n";
        // for(int i = 0; i < calledFunctionSoA.size(); i++)
        // {
        //   vector<int> indices = get<1>(calledFunctionSoA.at(i));
        //   // errs()<<get<0>(calledFunctionSoA.at(i))<<" uses ";
        //   for(int j = 0; j < indices.size(); j++)
        //   {
        //     // errs()<<"index "<<indices.at(j)<<"\n";
        //   }
        // }

        // errs()<<potentialSoA.size()<<"\n";
        // errs()<<confirmedSoA.size()<<"\n";

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
            // errs()<<"Searching "<<get<0>(searchFunc)<<"\n";
            if(argIndices.size() != 0)
            {
              argStoresSoA = getArgumentStoresSoA(&F,argIndices,type);
              // errs()<<"compare indices "<<argIndices.size()<<" to argStoresSoA "<<argStoresSoA.size()<<"\n";
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
                
                  // errs()<<op_str<<"\n";

                  for(auto it = potentialSoA.begin(); it != potentialSoA.end(); it++)
                  {
                    Value* aos = get<0>(*it);
                    std::string ptr_string; 
                    raw_string_ostream ptr(ptr_string);
                    aos->printAsOperand(ptr);

                    if((op_str.find("getelementptr") != std::string::npos) & (op_str.find(ptr_string) != std::string::npos))
                    {
                      StructType* structure =  get<3>(eraseFromPotentialSoA(aos));
                      nonPointerCount++;
                      confirmedSoA.push_back(make_tuple(aos,originFunctionSoA,"static",structure,false,false));
                      found = true;
                      break;
                    }
                  }

                  if(found == false)
                  {
                    for(auto it = potentialSoA.begin(); it != potentialSoA.end(); it++)
                    {
                      Value* aos = get<0>(*it);
                      string type = get<2>(*it);
                      StructType* structure = get<3>(*it);

                      if(loadedAoS == aos) //an potential AoS could be stored into an empty pointer - add this pointer to 'potential' list
                      {
                        eraseFromPossibleGlobalsSoA(SI->getOperand(1));
                        potentialSoA.push_back(make_tuple(SI->getOperand(1),originFunctionSoA,type,structure));
                        found = true;
                        break;
                      }
                    }

                    for(auto it = confirmedSoA.begin(); it != confirmedSoA.end(); it++)
                    {
                      Value* aos = get<0>(*it);
                      string type = get<2>(*it);
                      StructType* structure = get<3>(*it);

                      if(loadedAoS == aos) //an potential AoS could be stored into an empty pointer - add this pointer to 'potential' list
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
                        // errs()<<"Size :"<<constIntValue<<"\n";

                        // StructType* SoA = nullptr;

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
                    // errs()<<"yh\n";
                  }
                }
                else if(auto *GEP = dyn_cast<GetElementPtrInst>(&I))
                {
                  // GEP->print(errs());
                  // errs()<<"\n";
                  if((toFind.find(GEP->getSourceElementType()) != toFind.end()))
                  {
                    bool found = false;
                    Value* aos;
                    string type;
                    for(auto it = potentialSoA.begin(); it != potentialSoA.end(); it++)
                    {
                      aos = get<0>(*it);
                      type = get<2>(*it);
                      if(checkGEPSoA(GEP,aos,false,type) == true)
                      {
                        found = true;
                        eraseFromPotentialSoA(aos);
                        break;
                      }
                    }
                    for(auto it = argStoresSoA.begin(); it != argStoresSoA.end(); it++)
                    {
                      aos = get<0>(*it);
                      type = get<2>(*it);
                      if(checkGEPSoA(GEP,aos,true,type) == true)
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
                            possibleGlobalsSoA.push_back(make_tuple(gepOperandSoA,originFunctionSoA,structure)); //@globalFive in dynamic_AoS.c called in main() function 
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        //remove duplicate function calls - store value as well
        // for(auto it = calledFunctionSoA.begin(); it != calledFunctionSoA.end(); it)
        // {
        //   tuple<string,vector<int>,string> val = *it;
        //   if((get<0>(val) == get<0>(searchFunc)) & (get<1>(val) == get<1>(searchFunc)) & (get<2>(val) == get<2>(searchFunc)))
        //   {
        //     errs()<<"Removing function: "<<get<0>(val)<<" with val:"<<get<2>(val)<<" and"<<get<2>(searchFunc)<<"\n";
        //     calledFunctionSoA.erase(it);
        //   }
        //   else
        //   {
        //     it++;
        //   }
        // }
        }
      }

      //correctly detect SoA used as function arguments

      /*

        pair<value,function> getOriginSoA(F,aos,M)
        {
          result = pair<F,aos>
          search thru all F,B,I
            if callInst found
              if call to F found & not found in searchedCISoA vector
                  searchedCIvector.append(call)
                  return pair<value,function> res = getOriginSoA(new F)
          end
             
          return result
        }

        for each aos in confirmedSoA
          current confirmedSoA value = temp;
          result;
          newAoS = current aos;
          newFunc = nullptr;
          if aos = a callInsts
            get the associated function F
            while(newAoS != callInst & newFunc == nullptr)
              pair<Value*,Function*> result = look for a function call to this function: getOriginSoA(F,aos,M)
              for each operand used in the function call
                check if the operand exists in potentialSoA and if function matches
                  if yes, delete from potentialSoA
                    newAOS = aos
              endfor
              newFunc = origin function
            endwhile

          update temp with new values from newValues
        endfor
      
      */

     

     for(int a = 0; a < confirmedSoA.size(); a++)
     {
      // errs()<<"AOS: "<<a<<"\n";
       auto currConfirmed = confirmedSoA.at(a);
       Value* aos = get<0>(currConfirmed);
       Function* func = get<1>(currConfirmed);
       pair<Value*,Function*> result;
       bool found = false;
       string type = "dynamic";
       

      if(auto *SI = dyn_cast<StoreInst>(aos))
      {
        //  errs()<<"To search call: "<<func->getName()<<"\n";
        searchedFunctionCallsSoA.push_back(func);
        while(found == false)
        {
          result = getOriginSoA(func,aos,&M);
          // errs()<<"------------RESULTS----------------\n";
          // result.first->print(errs());
          // errs()<<"\n";
          // result.second->printAsOperand(errs());
          // errs()<<"\n";
          // errs()<<"-----------------------------------\n";

          if(auto *SI = dyn_cast<StoreInst>(result.first)) //no callinst found
          {
            searchedFunctionCallsSoA.pop_back();
            func = searchedFunctionCallsSoA.at(searchedFunctionCallsSoA.size() - 1);
            // errs()<<"Checking "<<func->getName()<<" again\n";
          }
          else if(auto *CI = dyn_cast<CallInst>(result.first))
          {
            // CI->print(errs());
            // errs()<<"\n";
            // if(CI->getCalledFunction()->getReturnType()->isVoidTy() == false) //for functions without a void return type, get the following LoadInst which could be an SoA
            // {
            //   Value* toFind = nullptr;
            //   if(auto *newCI = dyn_cast<CallInst>(CI->getNextNode()))
            //   {
            //     toFind = newCI->getOperand(0); //memcpy function
            //   }
            //   else if(auto *SI = dyn_cast<LoadInst>(CI->getNextNode()))
            //   {
            //     toFind = SI->getPointerOperand(); //store inst
            //   }

            //   toFind->print(errs());

            //   for(auto it = potentialArgumentsSoA.begin(); it != potentialArgumentsSoA.end(); it)
            //   {
            //       Value* aos = get<0>(*it);
            //       Function* func = get<1>(*it);

            //       // errs()<<"Is it: ";
            //       // aos->print(errs());
            //       // errs()<<"\n";

            //       if(aos == toFind)
            //       {
            //         // errs()<<"FOUND AOS\n";
            //         eraseFromPotentialArgumentsSoA(aos);
            //         result.first = aos;
            //         result.second = func;
            //         type = get<2>(*it);
            //         found = true;
            //         break;
            //       }
            //       else
            //         it++;
            //     }

            //     if(found == false)
            //     {
            //       for(auto it = possibleGlobalsSoA.begin(); it != possibleGlobalsSoA.end(); it)
            //       {
            //         Value* aos = get<0>(*it);
            //         Function* func = get<1>(*it);

            //         // errs()<<"Is it: ";
            //         // aos->print(errs());
            //         // errs()<<"\n";

            //         if(aos == toFind)
            //         {
            //           // errs()<<"FOUND AOS\n";
            //           eraseFromPossibleGlobalsSoA(aos);
            //           result.first = aos;
            //           result.second = func;
            //           found = true;
            //           break;
            //         }
            //         else
            //           it++;
            //       }
            //     }
            
            //     if(found == true)
            //     {
            //       searchedCISoA.clear();
            //       searchedFunctionCallsSoA.clear();
            //       break;
            //     }
            //     else
            //     {
            //       // searchedCISoA.clear();
            //       func = result.second;
            //     }
            // }
            // else
            // {
              for(auto a = CI->arg_begin(); a != CI->arg_end(); a++)
              {
                Value* toFind = *a;
                if(auto *LI = dyn_cast<LoadInst>(toFind))
                  toFind = LI->getOperand(0);

                // errs()<<"Call inst operand: ";
                // toFind->print(errs());
                // errs()<<"\n";

                for(auto it = potentialArgumentsSoA.begin(); it != potentialArgumentsSoA.end(); it)
                {
                  Value* aos = get<0>(*it);
                  Function* func = get<1>(*it);

                  // errs()<<"Is it: ";
                  // aos->print(errs());
                  // errs()<<"\n";

                  if(aos == toFind)
                  {
                    // errs()<<"FOUND AOS\n";
                    eraseFromPotentialArgumentsSoA(aos);
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
                  for(auto it = possibleGlobalsSoA.begin(); it != possibleGlobalsSoA.end(); it)
                  {
                    Value* aos = get<0>(*it);
                    Function* func = get<1>(*it);

                    // errs()<<"Is it: ";
                    // aos->print(errs());
                    // errs()<<"\n";

                    if(aos == toFind)
                    {
                      // errs()<<"FOUND AOS\n";
                      eraseFromPossibleGlobalsSoA(aos);
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
                  searchedCISoA.clear();
                  searchedFunctionCallsSoA.clear();
                  break;
                }
                else
                {
                  // searchedCISoA.clear();
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
        Value* aos = get<0>(*it);
        if(findInConfirmedSoA(aos))
          possibleGlobalsSoA.erase(it);
        else
          it++;
      }

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

      vector<pair<StructType*, bool>> checkedStructs;

      //iterate through all structs in the confirmedSoA list, and determine whether it contains a ptr field - which could make it recursive
      for(int i = 0; i < confirmedSoA.size(); i++)
      {
        // if(get<5>(confirmedSoA.at(i)) == true)
        //   continue;

        StructType* structure = get<3>(confirmedSoA.at(i));

        /// for each used structure, store the size of each struct

        // origStructSizes.insert(make_pair(structure,TD->getStructLayout(structure)->getSizeInBytes()));
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
          // string type = get<2>(potentialSoA.at(i));
          raw_string_ostream aos(aos_str);
          raw_string_ostream func(func_str);
          raw_string_ostream stru(struct_str);
          get<0>(potentialSoA.at(i))->printAsOperand(aos);
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
          raw_string_ostream aos(aos_str);
          raw_string_ostream func(func_str);
          get<0>(possibleGlobalsSoA.at(i))->printAsOperand(aos);
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
          raw_string_ostream aos(aos_str);
          raw_string_ostream func(func_str);
          raw_string_ostream stru(struct_str);
          get<0>(confirmedSoA.at(i))->print(aos);
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
            errs()<<" - global "<<type<<" SoA\n";
          }
          errs()<<"\n";
        }

        errs()<<"\n----------------------- END OF SOA DETECTION -----------------------\n";
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