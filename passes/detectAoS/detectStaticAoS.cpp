//detect dynamic AoS
//get all structs
//create vector of sizes of all structs
//find function calls
  //check if function is malloc
    //get size operand
    //if size is in structSizes
      //dynamic AoS found


#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/DataLayout.h"

#include <string>

using namespace llvm;

namespace {

struct detectStaticAoS : public PassInfoMixin<detectStaticAoS> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {

        //Static AoS detection without flags
        int detect = 0; 
        int staticCount = 0;
        int dynamicCount = 0;
        int count = 0; //counts the number of AoS structures found

        llvm::DataLayout* dl = new llvm::DataLayout(&M);

        std::vector<StructType*> allStructs = (&M)->getIdentifiedStructTypes(); //get all structs
        std::vector<int> structSizes = {};
        errs()<<"structs: "<<allStructs.size()<<"\n";
        for(int i = 0; i < allStructs.size(); i++)
        {
            StructType* s = allStructs.at(i);
            // StructLayout* sl = DataLayout.getStructLayout(allStructs.at(i));
            int myStructSize = dl->getTypeStoreSize(s);
            if(s->hasName())
            {
              std::string structName = s->getName().str();
              // errs()<<structName<<"\n";
            }
            errs()<<myStructSize<<"\n";
            structSizes.push_back(myStructSize);
        }

        for (auto &F : M) //iterate through all functions in the Module and print their names
        { 
            // std::vector<StructType*> allStructs = (&M)->getIdentifiedStructTypes();
            // errs()<<allStructs.size()<<"\n";
            //errs()<<"In function: "<<F.getName()<<"\n";
            for (auto &B : F)  //iterate through all blocks in each function
            { 
                for (auto &I : B) //iterate through all instructions in each block
                { 
                    //errs()<<"Instruction: "<<I<<"\n";
                    //errs()<<"Opcode: "<<I.getOpcodeName()<<"\n";
                    //static AoS detection
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
                            //   errs()<<type_str<<"\n";
                            }
                        }
                        else if(type_str.find("struct") != std::string::npos & type_str.find("[") != std::string::npos & type_str.find("]") != std::string::npos) //identifying AoS with size given as int literal - if type contains word "struct"
                        {
                            //allocation size can be 1 or more
                            errs()<<"Static Array of Structs (AoS) found"<<"\n"; //static AoS found
                            staticCount++;
                        }
                    }

                    //dynamic AoS detection
                    if (auto *CI = dyn_cast<CallInst>(&I))
                    {
                      Function* fn = CI->getCalledFunction(); //get function that has been called

                      // Type* returnType = fn->getReturnType();

                      std::string fn_name_string; 
                      raw_string_ostream rso(fn_name_string);
                      fn->print(rso);

                      if(fn_name_string.find("@malloc") != std::string::npos) //if function is a malloc
                      {

                        // errs()<<fn_name_string<<"\n";

                        Value* operand = CI->getArgOperand(0); //get sizeof() operand
                        
                        //operand->printAsOperand(errs());

                        if(auto *op = dyn_cast<Instruction>(operand)) 
                        {
                          Value* operand = op->getOperand(1); //get sizeof() value
                          if(auto *size = dyn_cast<ConstantInt>(operand))
                          {
                            APInt sizeVal = size->getValue();
                            std::string sizeString; 
                            raw_string_ostream rso(sizeString);
                            sizeVal.print(rso,true);

                            int sizeInt = std::stoi(sizeString);
                            errs()<<"Size:" <<sizeInt<<"\n";

                            std::vector<int>::iterator index = std::find(structSizes.begin(), structSizes.end(), sizeInt);
                            if(index != structSizes.end()) //if size matches size of struct
                            {
                              errs()<<"Dynamic Array of Structs (AoS) found"<<"\n"; //dynamic AoS found
                              dynamicCount++;
                              structSizes.erase(index); 
                            }
                          }
                        }

                      }



                        //errs()<<"yes"<<"\n";
                      //   Type* returnType = fn->getReturnType();
                      // returnType->print(errs());

                      //  FunctionType* ft = CI->getFunctionType();
                      // ft->print(errs());
                      // }


                      //   //get return type
                      //   Type* returnType = fn->getReturnType();
                      //   returnType->print(errs());
                      //   PointerType* PT = cast<PointerType>(returnType);
                      //   //PT->dump();
                      //   // Type *pointeeType = returnType->getType();
                      //   if(PT->isPointerTy())
                      //   {
                      //     errs()<<"tes\n";
                      //   }
                      // }



                    }

                }
            }
        }
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
          if(Name == "detectStaticAoS"){ //name of pass
            MPM.addPass(detectStaticAoS());
            return true;
          }
          return false;
        }
      );
    }
  };
}
     