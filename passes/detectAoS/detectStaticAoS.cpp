//iterate through module
//iterate though block
//iterate through instructions
    //if detect = 1
        //print out instructions
    //constantly check for load instruction
    //read operands from left to right
        //if contains AoS_Start
            //detect = 1
        //if contains AoS_End
            //detect = 0

#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {

struct detectStaticAoS : public PassInfoMixin<detectStaticAoS> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {

        //Static AoS detection without flags
        int detect = 0; 
        int count = 0; //counts the number of AoS structures found
        for (auto &F : M) //iterate through all functions in the Module and print their names
        { 
            //errs()<<"In function: "<<F.getName()<<"\n";
            for (auto &B : F)  //iterate through all blocks in each function
            { 
                for (auto &I : B) //iterate through all instructions in each block
                { 
                    //errs()<<"Instruction: "<<I<<"\n";
                    //errs()<<"Opcode: "<<I.getOpcodeName()<<"\n";
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
                              count++;
                            //   errs()<<type_str<<"\n";
                            }
                        }
                        else if(type_str.find("struct") != std::string::npos & type_str.find("[") != std::string::npos & type_str.find("]") != std::string::npos) //identifying AoS with size given as int literal - if type contains word "struct"
                        {
                            //allocation size can be 1 or more
                            errs()<<"Static Array of Structs (AoS) found"<<"\n"; //static AoS found
                            count++;
                        }
                    }
                }
            }
        }
        errs()<<"Number of static AoS data structures: " << count <<"\n";
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
     