#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;
using namespace std;

namespace {


vector<StructType*> toFind; //list of SoA to find inside functions 

bool checkType(Type* ty)
{
  for(int i = 0; i < toFind.size();  i++)
  {
    if(ty == toFind.at(i))
    {
      errs()<<"SoA found!\n";
      return true;
    }
  }
  return false;
}

vector<Value*> potential;
vector<Value*> confirmed; //good to store functions as well - as a pair (Value*, Function*)

//For Function Pass, use run(Function &F, FunctionAnalysisManager &FM)
struct detectSoA : public PassInfoMixin<detectSoA> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {

        // errs()<<"This is a Detect SoA pass\n";

        //iterate through identified structs

        vector<StructType*> allStructs = M.getIdentifiedStructTypes(); //get all struct types used in program

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
              errs()<<" is not a SoA\n";
              break;
            }
          }

          if(allFieldsArrays == true)
          {
            currStruct->print(errs());
            errs()<<" added to SoA toFind list\n";

            toFind.push_back(currStruct);
          }

        }

        //go through all global variables and find global SoA struct definitions in "toFind" list
        for(auto Global = M.global_begin(); Global != M.global_end(); ++Global)
        {
          Constant* constValue; 
          constValue = Global->getInitializer(); //get the initializer

          Type* t = constValue->getType(); //get the type of the global
          string type_str;
          raw_string_ostream tstr(type_str);
          t->print(tstr);

          if(checkType(t) == true)
          {
            Value* SoA = cast<Value>(&(*Global)); //store as Value in vector
            errs()<<"Adding SoA ";
            Global->print(errs());
            errs()<<" to confirmed list\n";
            confirmed.push_back(SoA);
          }
          
        }

        for (auto &F : M) //iterate through all functions in the Module
        { 
          for (auto &B : F) //iterate through all BasicBlocks in the Function
          {
            for (auto &I : B) //iterate through all Instructions in the Function
            { 
                // check for alloca and see if allocated element is a struct in toFind - this is a normal SoA struct definition
                if(auto *AI = dyn_cast<AllocaInst>(&I))
                {
                  Type* allocatedType = AI->getAllocatedType(); //get allocated type

                  if(checkType(allocatedType)) //if allocated type is a struct in toFind vector, add to confirmed
                  {
                    Value* SoA = cast<Value>(AI); //store as Value in vector
                    SoA->printAsOperand(errs());
                    errs()<<" is a local SoA!\n";
                    confirmed.push_back(SoA); //store this alloca instruction in confirmed
                  }
                }
                // else if(auto *GEP = dyn_cast<GetElementPtrInst>(&I))
                // {
                //   Type* resultElement = GEP->getResultElementType();
                //   if(checkType(resultElement)) //if GEP accessed 
                //   {

                //   }
                // }

                // check for malloc/calloc and find out if struct in toFind is being allocated (size 1, SoA, size > 1, AoSoA) - either pointer SoA or AoSoA (local or global)
            }
          }  
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
    LLVM_PLUGIN_API_VERSION, "detectSoA", "v0.1",
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, ModulePassManager &MPM, //For FunctionPass use FunctionPassManager &FPM
        ArrayRef<PassBuilder::PipelineElement>) {
          if(Name == "detectSoA"){ 
            MPM.addPass(detectSoA());
            return true;
          }
          return false;
        }
      );
    }
  };
}