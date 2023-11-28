//detect dynamic AoS - OLD!!
//get all structs
//create vector of sizes of all structs
//find function calls
  //check if function is malloc
    //get size operand
    //if size is in structSizes
      //dynamic AoS found

/////////////////////////////////////////////

//detect dynamic AoS - requires the arrays to be populated with data to confidently detect them
//iterate through main function
  //find malloc DONE
  //get next instruction
    //check if its a store
    //get first operand - a ptr- this is where the array is stored - save this % value in a vector (possibleAoS)
//continue iterating through main function
  //find GetElementPtrInst DONE
    //get first operand
    //check if load instruction
      //get first operand
      //if equal to array ptr - % value
        //if return type is struct
          //confidently detected a dynamic AoS!!!!!!!!!!!!!!!!!!
  //find CallInst if size of vector > 1 & ignoring "free" functions
    //check each operands & note operands no.
      //check if load instruction
        //get first operand of load instruction
        //if equal to array ptr - % value
          //save function name and operands number as a pair in a vector - to be searched later
    
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

void printAoS(vector<Value*> list)
{
  for(int i = 0; i < list.size(); i++)
  {
    list.at(i)->print(errs());
    errs()<<"\n";
  }
}

void removeFromPossibleAoSList(vector<Value*> &list, Value* remove)
{
  for (auto it = list.begin(); it != list.end();it++)
  {
    if(*it == remove)
    {
      list.erase(it);
      break;
    }
  }
}

void printFinalAoS(vector<Value*>& list)
{
  for(int i = 0; i < list.size(); i++)
  {
    list.at(i)->print(errs());
    errs()<<"\n";
  }
}

bool searchPossibleAoS(vector<Value*>& list, string name, bool addToFinal, vector<Value*>& finalList)
{
  for (auto it = list.begin(); it != list.end();it++)
  {
    Value* aos = *it;
    std::string aos_string; 
    raw_string_ostream aos_stream(aos_string);
    aos->printAsOperand(aos_stream);
    if(aos_string.find(name) != std::string::npos)
    {
        if(addToFinal == true)
        {
          finalList.push_back(aos);
          list.erase(it);
        }
        else
        {
          finalList.push_back(aos); //going to popped out soon
        }
        
        return true;
    }

  }
  return false;
}



namespace {

struct detectAoS : public PassInfoMixin<detectAoS> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {

        //Static AoS detection without flags
        int detect = 0; 
        int staticCount = 0;
        int dynamicCount = 0;

        vector<Value*> possibleAoSList; //vector which stores pointer locations of possible AoS - these need to be inspected and confirmed
        vector<Value*> AoS_Values; //vector which stores pointer locations of possible AoS - these need to be inspected and confirmed
        deque<tuple<string, int, Value*>> functionInspectionList; //vector that stores functions that need to be inspected for GPE instructions in order to fully determine that a dynamic AoS exists 

        for (auto &F : M) //iterate through all functions in the Module and print their names
        { 
            // errs()<<"In function: "<<F.getName()<<"\n";
            if(F.getName().find("main") != std::string::npos)
            {
              for (auto &B : F)  //iterate through all blocks in each function
              { 
                  for (auto &I : B) //iterate through all instructions in each block
                  { 
                      //static AoS detection
                      if(possibleAoSList.size() != 0) //if probable AoS pointers are found
                          {
                            if (auto *GEP = dyn_cast<GetElementPtrInst>(&I)) //look for getelementptr instructions 
                            {
                              Type* returnType = GEP->getResultElementType(); //get return type of this GEP instruction

                              if(returnType->isStructTy()) //return type should be a Struct
                              {
                                // returnType->printAs(errs());
                                Value* operand = GEP->getOperand(0);
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

                                  if(searchPossibleAoS(possibleAoSList, op_string, true, AoS_Values)) //if loaded operand is one of the malloc-ed arrays detected earlier
                                  {
                                    errs()<<"Dynamic Array of Structs (AoS) found"<<"\n"; //dynamic AoS found
                                    dynamicCount++;
                                  }
                                }
                              }
                            }
                          }
                      
                      if (auto *AI = dyn_cast<AllocaInst>(&I)) //check for alloca instructions - which allocates memory to the stack
                      {
                          Type* t = AI->getAllocatedType(); //returns type that is being allocated by the instruction
                          //Get type as a string
                          std::string type_str; 
                          raw_string_ostream rso(type_str);
                          t->print(rso);
                          
                          if(AI->isArrayAllocation()) //identifying AoS with size given as a variable - if the alloca instruction has a size attribute that is not 1 (i.e. not a single allocation of the type)
                          {
                              if(t->isStructTy()) //if the allocated type is a struct
                              {
                                errs()<<"Static Array of Structs (AoS) found"<<"\n"; //static AoS found
                                staticCount++;

                                Value* AoS = cast<Instruction>(AI); //store as Value in vector
                                // AoS->print(errs());
                                AoS_Values.push_back(AoS);

                              //   errs()<<type_str<<"\n";
                              }
                          }
                          else if(type_str.find("struct") != std::string::npos & type_str.find("[") != std::string::npos & type_str.find("]") != std::string::npos) //identifying AoS with size given as int literal - if type contains word "struct"
                          {
                              //allocation size can be 1 or more
                              errs()<<"Static Array of Structs (AoS) found"<<"\n"; //static AoS found
                              staticCount++;

                              Value* AoS = cast<Instruction>(AI); //store as Value in vector
                              // AoS->print(errs());
                              AoS_Values.push_back(AoS);
                          }
                      }

                      //dynamic AoS detection
                
                      if (auto *CI = dyn_cast<CallInst>(&I))
                      {
                          Function* fn = CI->getCalledFunction(); //get function that has been called
                          FunctionType* ft = fn->getFunctionType();
            
                          std::string fn_name_string = fn->getName().str();

                          if(fn_name_string.find("malloc") != std::string::npos) //if function is a malloc
                            detect = 1; //set this flag to 1 when a malloc is found  
                          
                          else if((possibleAoSList.size() != 0) & (fn_name_string.find("free") == std::string::npos)) //if any probable AoS has been detected, look out for any functions that use it, except free()
                          {
                              //errs()<<"Func name:"<<fn_name_string<<"\n";

                              int argNo = 0; //store index of argument

                              for(auto a = CI->arg_begin(); a != CI->arg_end(); a++) //get all function arguments
                              {
                                Value* argument = a->get(); //get argument of function at index: argNo

                                // errs()<<"asd\n";
                                // argument->printAsOperand(errs());

                                //check if each argument Value is a Load instance and checks if loaded operand is one of the malloc-ed arrays detected earlier
                                if(auto LI = dyn_cast<LoadInst>(argument))
                                {
                                  Value* operand = LI->getOperand(0); //get first operand (value being loaded)
                                  //operand->print(errs());
                                  
                                  //convert to string for comparison
                                  std::string op_string; 
                                  raw_string_ostream ops(op_string);
                                  operand->printAsOperand(ops);

                                  if(searchPossibleAoS(possibleAoSList, op_string, false, AoS_Values)) //if this function loads a malloc-ed array, store its function name for inspection
                                  {
                                      // errs()<<op_string<<"\n";
                                      //errs()<<"added function "<<fn_name_string<<" to inspection list\n";
                                      functionInspectionList.push_back(make_tuple(fn_name_string, argNo, AoS_Values.back()));
                                      AoS_Values.pop_back();
                                  }

                                }
                                argNo++;
                              }
                          }
                      
                          
                      }

                      if(detect == 1) //if a malloc was found, check the following store instruction which stores the malloc-ed pointer
                      {
                        if (auto *SI = dyn_cast<StoreInst>(&I))
                        {
                            Value* operand = SI->getOperand(1); //get second operand of this store instruction

                            //getting operand as string
                            // std::string op_string; 
                            // raw_string_ostream ops(op_string);
                            // operand->printAsOperand(ops);
                            // operand->printAsOperand(errs());

                            possibleAoSList.push_back(operand);
                            detect = 0; //turn off flag
                        }
                      }

                  }
              }
            }
        }
        //FIx THIS - an array can be populated by the same function - this ignores duplicate functions when inspected - better off popping function from vector, iterating through functions and finding it, then rinse and repeat
        
        while(!functionInspectionList.empty())
        {
            tuple<string,int,Value*> func = functionInspectionList.at(0);
            functionInspectionList.pop_front();

            string funcName = get<0>(func);
            int argIndex = get<1>(func);
            Value* possible_AoS = get<2>(func);
            errs()<<"Curr function:"<<funcName<<"\n";

            bool arrayStoreFound = false; //flag which indicates whether the variable, which stores the array, is found inside the function
            bool successDetect = false; //used to break out of for loop early once a AoS is detected inside a function

            string array_store_string; 
            raw_string_ostream store(array_store_string);

          for (auto &F : M) //iteration of functions to determine if any remaining arrays in vector are dynamic AoS
          {
              //inspect current function
              if(F.getName().str() == funcName)
              {
                Argument* first = F.arg_begin()+argIndex;

                //get argument as string
                std::string arg_string; 
                raw_string_ostream args(arg_string);
                first->printAsOperand(args);
                //errs()<<arg_string<<"\n";

                Value* arrayStore;

                for (auto &B : F)  //iterate through all blocks in each function
                { 
                    if(successDetect)
                      break;

                    for (auto &I : B) //iterate through all instructions in each block
                    {                       
                    
                      //look out for StoreInst if 
                      if(arrayStoreFound == false)
                      {
                        if (auto *SI = dyn_cast<StoreInst>(&I))
                        {
                          Value* operand = SI->getOperand(0); //get first operand of this store instruction

                          //getting operand as string
                          std::string op_string; 
                          raw_string_ostream ops(op_string);
                          operand->printAsOperand(ops);

                          if(op_string.find(arg_string) != std::string::npos)
                          {
                            arrayStore = SI->getOperand(1); //get second operand of this store instruction

                            arrayStore->printAsOperand(store);
                            // errs()<<array_store_string<<"\n";

                            arrayStoreFound = true;
                          }
                        }
                      }
                      //with the array store found, now look for GPE instruction
                      else if(arrayStoreFound == true)
                      {
                          if (auto *GEP = dyn_cast<GetElementPtrInst>(&I)) //look for getelementptr instructions 
                          {
                            Type* returnType = GEP->getResultElementType(); //get return type of this GEP instruction

                            if(returnType->isStructTy()) //return type should be a Struct
                            {
                              // returnType->printAs(errs());
                              Value* operand = GEP->getOperand(0);
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

                                //errs()<<op_string<<"\n";

                                if(op_string == array_store_string) //if loaded operands equals the operands of array called in to function
                                {
                                    errs()<<"Dynamic Array of Structs (AoS) found"<<"\n"; //dynamic AoS found
                                    dynamicCount++;
                                    AoS_Values.push_back(possible_AoS);
                                    removeFromPossibleAoSList(possibleAoSList,possible_AoS);
                                    successDetect = true; //no longer need to search further in this function
                                    break; //exit function
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

        vector<pair<Value*, string>> checkFuncInMainList;

        //if a AoS still exists in this list and has not been confirmed as a dynamic AoS, maybe it is a global variable
        if(possibleAoSList.size() != 0)
        {   
          for (auto &F : M) //iteration of functions to determine if the global variables are dynamic AoS
          {
            for(auto it = possibleAoSList.begin(); it < possibleAoSList.end(); it++)
            {
              Value* aos = *it;
              std::string aos_string; 
              raw_string_ostream aos_stream(aos_string);
              aos->printAsOperand(aos_stream);
              // errs()<<aos_string<<"\n";

              if(auto GV = dyn_cast<GlobalVariable>(aos)) //if AoS is a global variable
              {
                // errs()<<"In function: "<<F.getName()<<"\n";
                for (auto &B : F) //iterate through all blocks in each function
                { 
                  for (auto &I : B) //iterate through all instructions in each block
                  {  
                    if (auto *GEP = dyn_cast<GetElementPtrInst>(&I)) //look for getelementptr instructions 
                      {
                        Type* returnType = GEP->getResultElementType(); //get return type of this GEP instruction

                        if(returnType->isStructTy()) //return type should be a Struct
                        {
                          // returnType->printAs(errs());
                          Value* operand = GEP->getOperand(0);
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

                            //errs()<<op_string<<"\n";

                            string funcName = F.getName().str();

                            if(op_string == aos_string) //if loaded operands equals the operands of array called in to function
                            {
                                // errs()<<"Dynamic Array of Structs (AoS) found"<<"\n"; //dynamic AoS found
                                // dynamicCount++;
                                // AoS_Values.push_back(aos);
                                // removeFromPossibleAoSList(possibleAoSList,aos);
                                checkFuncInMainList.push_back(make_pair(aos,funcName));
                                break; //exit function
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

        if(checkFuncInMainList.size() > 0)
        {
          for (auto &F : M) //iterate through all functions in the Module and print their names
          { 
              // errs()<<"In function: "<<F.getName()<<"\n";
              if(F.getName().find("main") != std::string::npos)
              {
                for (auto &B : F)  //iterate through all blocks in each function
                { 
                    for (auto &I : B) //iterate through all instructions in each block
                    { 
                        if(auto *CI = dyn_cast<CallInst>(&I))
                        {
                          Function* f = CI->getCalledFunction();
                          string funcName = f->getName().str();

                          for(auto it = checkFuncInMainList.begin(); it < checkFuncInMainList.end(); it++)
                          {
                            if(it->second == funcName) //if function name is found in main
                            {
                              errs()<<"Dynamic Array of Structs (AoS) found (global)"<<"\n"; //dynamic AoS found
                              dynamicCount++;
                              AoS_Values.push_back(it->first);
                              removeFromPossibleAoSList(possibleAoSList,it->first);
                              checkFuncInMainList.erase(it);
                              break;
                            }
                          }
                        }
                    }
                }
              }
          }
        }

        errs()<<"Possible AoS\n";
        printAoS(possibleAoSList);
        errs()<<"\n";
        errs()<<"Final AoS\n";
        printFinalAoS(AoS_Values);

        errs()<<"Number of static AoS data structures: " << staticCount <<"\n";
        errs()<<"Number of dynamic AoS data structures: " << dynamicCount <<"\n";

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
     