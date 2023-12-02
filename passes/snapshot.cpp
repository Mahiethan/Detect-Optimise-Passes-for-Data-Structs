//THIS VERSION DOES NOT DETECT STATIC OR DYNAMIC CORRECTLY


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
vector<pair<Value*,Function*>> potential;
vector<Value*> argStores;
vector<pair<Value*,Function*>> possibleGlobals;
vector<pair<Value*,Function*>> confirmed;
vector<tuple<string,vector<int>,Value*>> calledFunction; //stores pair of function name and used argument index of pointer (if any)

Function* originFunction = NULL;

int staticCount = 0;
int dynamicCount = 0;
bool mallocFlag = false;

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
        // AoS->print(errs());
        potential.push_back(make_pair(AoS,originFunction));

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

      Value* AoS = cast<Instruction>(AI); //store as Value in vector
      // AoS->print(errs());
      potential.push_back(make_pair(AoS,originFunction));
      }
  }
}

void eraseFromConfirmed(Value* val)
{
  for(auto it = confirmed.begin(); it != confirmed.end(); it)
  {
    Value* aos = it->first;
    std::string aos_string; 
    raw_string_ostream aos_stream(aos_string);
    aos->printAsOperand(aos_stream);
    if(aos == val)
    {
      // errs()<<"ERasing from potential\n";
      //  errs()<<"Adding back to potential: "<<aos_string<<"\n";
      confirmed.erase(it);
      dynamicCount--;
    }
    else
    {
    it++;
    }
  }
}

void eraseFromPossibleGlobals(Value* val)
{
  for(auto it = possibleGlobals.begin(); it != possibleGlobals.end(); it)
  {
    Value* aos = it->first;
    // std::string aos_string; 
    // raw_string_ostream aos_stream(aos_string);
    // aos->printAsOperand(aos_stream);
    if(aos == val)
    {
      // errs()<<"ERasing from potential\n";
      possibleGlobals.erase(it);
    }
    else
    {
    it++;
    }
  }
}

void getPotential(Instruction* I)
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
      aos = it->first;
      func = it->second;
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
      eraseFromPossibleGlobals(operand);
      confirmed.push_back(make_pair(aos,func));
      dynamicCount++;
    }
    else
    {
      for(auto it = potential.begin(); it != potential.end(); it)
      {
        aos = it->first;
        func = it->second;
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
        eraseFromConfirmed(operand); //remove from confirmed list if it exists before adding it to potential
        potential.push_back(make_pair(operand,originFunction));
      }
    }
  }  
  mallocFlag = false; //disable flag - don't search for StoreInst after malloc
}

void eraseFromPotential(Value* val)
{
  for(auto it = potential.begin(); it != potential.end(); it)
  {
    Value* aos = it->first;
    // std::string aos_string; 
    // raw_string_ostream aos_stream(aos_string);
    // aos->printAsOperand(aos_stream);
    if(aos == val)
    {
      // errs()<<"ERasing from potential\n";
      potential.erase(it);
    }
    else
    {
    it++;
    }
  }
}

void eraseFromArgStores(Value* val)
{
  for(auto it = argStores.begin(); it != argStores.end(); it)
  {
    Value* aos = *it;
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
    }
    else
    {
    it++;
    }
  }
}

Value* gepOperand = NULL;
Value* calledAoS = NULL;

bool checkGEP(GetElementPtrInst *gep, Value* aos, bool isParam)
{
  
    Value* operand = gep->getOperand(0);

    //getting operand as string
    std::string op_string; 
    raw_string_ostream ops(op_string);
    operand->printAsOperand(ops);

    // errs()<<"Checking operand: "<<op_string<<"\n";

    while(isa<LoadInst>(operand) == false & isa<AllocaInst>(operand) == false)
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

    if(auto *AI = dyn_cast<AllocaInst>(operand))
    {
      std::string ptr_string; 
      raw_string_ostream ptr(ptr_string);
      aos->printAsOperand(ptr);

      // errs()<<"does it equal "<<ptr_string<<"\n";

      if(op_string == ptr_string) //if loaded operand is one of the malloc-ed arrays detected earlier
      {
        // errs()<<"Dynamic Array of Structs (AoS) found"<<"\n"; //dynamic AoS found
        if(isa<AllocaInst>(aos) == true)
          staticCount++;
        else
          dynamicCount++;

        if(isParam)
          confirmed.push_back(make_pair(calledAoS,originFunction));
        else
          confirmed.push_back(make_pair(aos,originFunction));
        eraseFromPotential(aos);
        eraseFromPossibleGlobals(aos);
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
      operand->printAsOperand(ops);

        // errs()<<"for operand: "<<op_string<<"\n";
      

      std::string ptr_string; 
      raw_string_ostream ptr(ptr_string);
      aos->printAsOperand(ptr);

      // errs()<<"does it equal "<<ptr_string<<"\n";

      if(op_string == ptr_string) //if loaded operand is one of the malloc-ed arrays detected earlier
      {
        // errs()<<"Dynamic Array of Structs (AoS) found"<<"\n"; //dynamic AoS found
        if(isa<AllocaInst>(aos) == true)
          staticCount++;
        else
          dynamicCount++;

        if(isParam)
          confirmed.push_back(make_pair(calledAoS,originFunction));
        else
          confirmed.push_back(make_pair(aos,originFunction));
        eraseFromPotential(aos);
        eraseFromPossibleGlobals(aos);
        // errs()<<"Size of argStores: "<<argStores.size()<<"\n";
        // errs()<<"Size of potential: "<<potential.size()<<"\n";
        return true;
      }
      gepOperand = operand;

    }
  
  return false;
}

void getCalledFunctions(CallInst* CI)
{
  vector<int> indices;
  Value* aos;
  Value* paramAoS;
  int index = 0;
  Function* f = CI->getCalledFunction();
  string funcName = f->getName().str();
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
        aos = potential.at(i).first;
        std::string value_string; 
        raw_string_ostream aos_stream(value_string);
        aos->printAsOperand(aos_stream);
        // errs()<<"pointer: "<<value_string<<"\n";
        // errs()<<"operand: "<<op_string<<"\n";
        if(value_string == op_string)
        {
          // errs()<<"adding "<<value_string<<" to indices\n";
          indices.push_back(index);
          eraseFromPotential(aos);
          detect = true;
          paramAoS = aos;

          break;
        }
      }
      if(detect == false)
      {
        for(int i = 0; i < argStores.size(); i++)
        {
          aos = argStores.at(i);
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
        if(auto *GV = dyn_cast<GlobalVariable>(operand))
        {
          possibleGlobals.push_back(make_pair(operand,f));
          detect = true;
        }
      }
    
    index++;
  }
  // errs()<<"Size of indices: "<<indices.size()<<"\n";
  // errs()<<"Adding function: "<<funcName<<"with indices size"<<indices.size()<<"that operates on"<<valName<<"\n";
  calledFunction.push_back(std::make_tuple(funcName,indices,paramAoS));
}

vector<Value*> getArgumentStores(Function *F, vector<int> indices)
{
  vector<Value*> arguments;
  vector<Value*> newStores;
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
              newStores.push_back(arrayStore);
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

        vector<Value*> possibleAoSList; //vector which stores pointer locations of possible AoS - these need to be inspected and confirmed
        vector<Value*> AoS_Values; //vector which stores pointer locations of possible AoS - these need to be inspected and confirmed
        deque<tuple<string, int, Value*>> functionInspectionList; //vector that stores functions that need to be inspected for GPE instructions in order to fully determine that a dynamic AoS exists

        // errs()<<"Getting globals\n";
        for(GlobalVariable& gv : M.globals())
        {
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

              Value* AoS = cast<Value>(&gv); //store as Value in vector
              // AoS->printAsOperand(errs());
              potential.push_back(make_pair(AoS,originFunction));
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
                      Value* operand = SI->getOperand(1);
                      std::string op_str; 
                      raw_string_ostream to_str(op_str);
                      operand->printAsOperand(to_str);

                      errs()<<op_str<<"\n";

                      for(auto it = potential.begin(); it != potential.end(); it++)
                      {
                        Value* aos = it->first;
                        std::string ptr_string; 
                        raw_string_ostream ptr(ptr_string);
                        aos->printAsOperand(ptr);

                        if((op_str.find("getelementptr") != std::string::npos) & (op_str.find(ptr_string) != std::string::npos))
                        {
                          eraseFromPotential(aos);
                          staticCount++;
                          confirmed.push_back(make_pair(aos,originFunction));
                          break;
                        }
                      }

                    }
                    else if(auto *AI = dyn_cast<AllocaInst>(&I))
                      detectStaticAoS(AI);
                    else if(auto *CI = dyn_cast<CallInst>(&I))
                    {
                      Function* f = CI->getCalledFunction();
                      string funcName = f->getName().str();
                      if(funcName == "malloc" | funcName == "calloc")
                        mallocFlag = true;
                      else if(funcName != "free")
                        getCalledFunctions(CI);
                    }
                    else if(auto *GEP = dyn_cast<GetElementPtrInst>(&I))
                    {
                      bool found = false;
                      Value* aos;
                      if(GEP->getResultElementType()->isStructTy())
                      {
                        for(auto it = potential.begin(); it != potential.end(); it++)
                        {
                          aos = it->first;
                          std::string ptr_string; 
                          raw_string_ostream ptr(ptr_string);
                          aos->printAsOperand(ptr);
                          // errs()<<"Searching for "<<ptr_string<<"\n";
                          if(checkGEP(GEP,aos,false) == true)
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
                            possibleGlobals.push_back(make_pair(gepOperand,originFunction));
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
        for(int i = 0; i < calledFunction.size(); i++)
        {
          vector<int> indices = get<1>(calledFunction.at(i));
          // errs()<<get<0>(calledFunction.at(i))<<" uses ";
          for(int j = 0; j < indices.size(); j++)
          {
            // errs()<<"index "<<indices.at(j)<<"\n";
          }
        }

        // errs()<<potential.size()<<"\n";
        // errs()<<confirmed.size()<<"\n";

      while(calledFunction.size() != 0)
      {
        tuple<string,vector<int>,Value*> searchFunc = calledFunction.back();
        calledFunction.pop_back();
        vector<int> argIndices = get<1>(searchFunc);
        calledAoS = get<2>(searchFunc); //get AoS that was used as parameter in function call
        for (auto &F : M) //iterate through all functions in the Module and print their names
        { 
          if(F.getName() == get<0>(searchFunc))
          {
            originFunction = &F;
            // errs()<<"Searching "<<get<0>(searchFunc)<<"\n";
            if(argIndices.size() != 0)
            {
              argStores = getArgumentStores(&F,argIndices);
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

                  errs()<<op_str<<"\n";

                  for(auto it = potential.begin(); it != potential.end(); it++)
                  {
                    Value* aos = it->first;
                    std::string ptr_string; 
                    raw_string_ostream ptr(ptr_string);
                    aos->printAsOperand(ptr);

                    if((op_str.find("getelementptr") != std::string::npos) & (op_str.find(ptr_string) != std::string::npos))
                    {
                      eraseFromPotential(aos);
                      staticCount++;
                      confirmed.push_back(make_pair(aos,originFunction));
                      break;
                    }
                  }

                }
                else if(auto *AI = dyn_cast<AllocaInst>(&I))
                  detectStaticAoS(AI);
                else if(auto *CI = dyn_cast<CallInst>(&I))
                {
                  Function* f = CI->getCalledFunction();
                  string funcName = f->getName().str();
                  if(funcName == "malloc" | funcName == "calloc")
                    mallocFlag = true;
                  else if(funcName != "free")
                  {
                    getCalledFunctions(CI);
                  }
                }
                else if(auto *GEP = dyn_cast<GetElementPtrInst>(&I))
                {
                  if(GEP->getResultElementType()->isStructTy())
                  {
                    bool found = false;
                    Value* aos;
                    for(auto it = potential.begin(); it != potential.end(); it++)
                    {
                      aos = it->first;
                      if(checkGEP(GEP,aos,false) == true)
                      {
                        found = true;
                        eraseFromPotential(aos);
                        break;
                      }
                    }
                    for(auto it = argStores.begin(); it != argStores.end(); it++)
                    {
                      aos = *it;
                      if(checkGEP(GEP,aos,true) == true)
                      {
                        found = true;
                        eraseFromArgStores(aos);
                        break;
                      }
                    }
                    if(found == false)
                    {
                      if(gepOperand != NULL)
                      {
                        if(auto *GV = dyn_cast<GlobalVariable>(gepOperand))
                        {
                          possibleGlobals.push_back(make_pair(gepOperand,originFunction));
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

        errs()<<"Size of potential list: "<<potential.size()<<"\n";
        for(int i = 0; i < potential.size(); i++)
        {
          std::string aos_str; 
          std::string func_str; 
          raw_string_ostream aos(aos_str);
          raw_string_ostream func(func_str);
          potential.at(i).first->printAsOperand(aos);
          errs()<<i<<": "<<aos_str;
          Function* funcName = potential.at(i).second;
          if(funcName != NULL)
          {
            funcName->printAsOperand(func);
            errs()<<" declared in function: "<<func_str<<"\n";
          }
          else
          {
            errs()<<" declared as a global variable\n";
          }
        }

        errs()<<"\nSize of possibleGlobals list: "<<possibleGlobals.size()<<"\n";
        for(int i = 0; i < possibleGlobals.size(); i++)
        {
          std::string aos_str; 
          std::string func_str; 
          raw_string_ostream aos(aos_str);
          raw_string_ostream func(func_str);
          possibleGlobals.at(i).first->printAsOperand(aos);
          errs()<<i<<": "<<aos_str;
          possibleGlobals.at(i).second->printAsOperand(func);
          errs()<<" used in function: "<<func_str<<"\n";

        }

        errs()<<"\nNumber of static AoS data structures: " << staticCount <<"\n";
        errs()<<"Number of dynamic AoS data structures: " << dynamicCount <<"\n";

        errs()<<"Size of confirmed list: "<<confirmed.size()<<"\n\n";
        for(int i = 0; i < confirmed.size(); i++)
        {
          std::string aos_str; 
          std::string func_str; 
          raw_string_ostream aos(aos_str);
          raw_string_ostream func(func_str);
          confirmed.at(i).first->printAsOperand(aos);
          errs()<<i<<": "<<aos_str;
          confirmed.at(i).second->printAsOperand(func);
          errs()<<" from function: "<<func_str<<"\n";


        }

        //Set to ::all() if IR is unchanged, otherwise ::none()
        return PreservedAnalyses::all();
    };
};
}


//Creates plugin for pass - required
extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "passName", "v0.1",
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, ModulePassManager &MPM, //Module pass
        ArrayRef<PassBuilder::PipelineElement>) {
          if(Name == "detectAoS"){ //name of pass
            MPM.addPass(detectAoS());
            return true;
          }
          return false;
        }
      );
    }
  };
}