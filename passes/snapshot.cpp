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
vector<Value*> potential;
vector<Value*> argStores;
vector<Value*> confirmed;
vector<tuple<string,vector<int>,string>> calledFunction; //stores pair of function name and used argument index of pointer (if any)


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
        errs()<<"Static Array of Structs (AoS) found"<<"\n"; //static AoS found
        staticCount++;

        Value* AoS = cast<Instruction>(AI); //store as Value in vector
        // AoS->print(errs());
        confirmed.push_back(AoS);

      //   errs()<<type_str<<"\n";
      }
  }
  else if(type_str.find("struct") != std::string::npos & type_str.find("[") != std::string::npos & type_str.find("]") != std::string::npos) //identifying AoS with size given as int literal - if type contains word "struct"
  {
      if(type_str.find("[0 x") == std::string::npos) //if allocated size is not 0
      {
      //allocation size can be 1 or more
      // errs()<<"Static Array of Structs (AoS) found"<<"\n"; //static AoS found
      staticCount++;

      Value* AoS = cast<Instruction>(AI); //store as Value in vector
      // AoS->print(errs());
      confirmed.push_back(AoS);
      }
  }
}

void getPotential(Instruction* I)
{
  if(auto *SI = dyn_cast<StoreInst>(I))
  {
    Value* operand = SI->getOperand(1); //get second operand of this store instruction
    potential.push_back(operand);

    std::string aos_string; 
    raw_string_ostream aos_stream(aos_string);
    operand->printAsOperand(aos_stream);
    // errs()<<"Potential "<<aos_string<<"\n";
  }  
  mallocFlag = false; //disable flag - don't search for StoreInst after malloc
}

void eraseFromPotential(Value* val)
{
  for(auto it = potential.begin(); it != potential.end(); it)
  {
    Value* aos = *it;
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

bool checkGEP(GetElementPtrInst *gep, Value* aos)
{
  Type* returnType = gep->getResultElementType(); //get return type of this GEP instruction
  if(returnType->isStructTy()) //return type should be a Struct
  {
    Value* operand = gep->getOperand(0);

    //getting operand as string
    std::string op_string; 
    raw_string_ostream ops(op_string);
    operand->print(ops);

    if(auto *OI = dyn_cast<LoadInst>(operand)) //check if instruction is a Load instruction
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
        dynamicCount++;
        confirmed.push_back(aos);
        // errs()<<"Size of argStores: "<<argStores.size()<<"\n";
        // errs()<<"Size of potential: "<<potential.size()<<"\n";
        return true;
      }

    }
  }
  return false;
}

void getCalledFunctions(CallInst* CI)
{
  vector<int> indices;
  Value* aos;
  string valName;
  int index = 0;
  Function* f = CI->getCalledFunction();
  string funcName = f->getName().str();
  // errs()<<"Checking func call "<<funcName<<"\n";
  for(auto a = CI->arg_begin(); a != CI->arg_end(); a++) //get all function arguments
  {
    Value* argument = a->get(); //get argument o
    if(auto LI = dyn_cast<LoadInst>(argument))
    {
      Value* operand = LI->getOperand(0); //get first operand (value being loaded)      
      //convert to string for comparison
      std::string op_string; 
      raw_string_ostream ops(op_string);
      operand->printAsOperand(ops);

      bool detect = false;

      for(int i = 0; i < potential.size(); i++)
      {
        aos = potential.at(i);
        std::string value_string; 
        raw_string_ostream aos_stream(value_string);
        aos->printAsOperand(aos_stream);
        // errs()<<"pointer: "<<value_string<<"\n";
        // errs()<<"operand: "<<op_string<<"\n";
        if(value_string == op_string)
        {
          // errs()<<"adding to indices\n";
          indices.push_back(index);
          eraseFromPotential(aos);
          detect = true;
          valName = value_string;

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
            indices.push_back(index);
            eraseFromArgStores(aos);
            valName = value_string;

            break;
          }
        }
      }
    }
    index++;
  }
  // errs()<<"Size of indices: "<<indices.size()<<"\n";
  // errs()<<"Adding function: "<<funcName<<"with indices size"<<indices.size()<<"that operates on"<<valName<<"\n";
  calledFunction.push_back(std::make_tuple(funcName,indices,valName));
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
        // for(GlobalVariable& gv : M.globals())
        // {
        //   potential.push_back(&gv);
        // }
        bool checkMain = false;
        for (auto &F : M) //iterate through all functions in the Module and print their names
        { 
            if(checkMain == true)
              break;
            // errs()<<"In function: "<<F.getName()<<"\n";
            if(F.getName().find("main") != std::string::npos)
            {
              for (auto &B : F)  //iterate through all blocks in each function
              { 
                  for (auto &I : B) //iterate through all instructions in each block
                  { 
                    if(mallocFlag == true)
                      getPotential(&I);
                    else if(auto *AI = dyn_cast<AllocaInst>(&I))
                      detectStaticAoS(AI);
                    else if(auto *CI = dyn_cast<CallInst>(&I))
                    {
                      Function* f = CI->getCalledFunction();
                      string funcName = f->getName().str();
                      if(funcName == "malloc")
                        mallocFlag = true;
                      else if(funcName != "free")
                        getCalledFunctions(CI);
                    }
                    else if(auto *GEP = dyn_cast<GetElementPtrInst>(&I))
                    {
                      for(auto it = potential.begin(); it != potential.end(); it++)
                      {
                        Value* aos = *it;
                        if(checkGEP(GEP,aos) == true)
                        {
                          eraseFromPotential(aos);
                          break;
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
        tuple<string,vector<int>,string> searchFunc = calledFunction.back();
        calledFunction.pop_back();
        vector<int> argIndices = get<1>(searchFunc);
        for (auto &F : M) //iterate through all functions in the Module and print their names
        { 
          if(F.getName() == get<0>(searchFunc))
          {
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
                else if(auto *AI = dyn_cast<AllocaInst>(&I))
                  detectStaticAoS(AI);
                else if(auto *CI = dyn_cast<CallInst>(&I))
                {
                  Function* f = CI->getCalledFunction();
                  string funcName = f->getName().str();
                  if(funcName == "malloc")
                    mallocFlag = true;
                  else if(funcName != "free")
                  {
                    getCalledFunctions(CI);
                  }
                }
                else if(auto *GEP = dyn_cast<GetElementPtrInst>(&I))
                {
                  for(auto it = potential.begin(); it != potential.end(); it++)
                  {
                    Value* aos = *it;
                    if(checkGEP(GEP,aos) == true)
                    {
                      eraseFromPotential(aos);
                      break;
                    }
                  }
                  for(auto it = argStores.begin(); it != argStores.end(); it++)
                  {
                    Value* aos = *it;
                    if(checkGEP(GEP,aos) == true)
                    {
                      eraseFromArgStores(aos);
                      break;
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
        errs()<<"Number of static AoS data structures: " << staticCount <<"\n";
        errs()<<"Number of dynamic AoS data structures: " << dynamicCount <<"\n";

        // errs()<<confirmed.size()<<"\n";
        // for(int i = 0; i < confirmed.size(); i++)
        // {
        //   std::string type_str; 
        //   raw_string_ostream to_str(type_str);
        //   confirmed.at(i)->printAsOperand(to_str);
        //   errs()<<i<<":"<<type_str<<"\n";

        // }

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