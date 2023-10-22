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

struct detectAoS : public PassInfoMixin<detectAoS> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {

        //AoS detection using flags
        for (auto &F : M) //iterate through all functions in the Module and print their names
        { 
            int detect = 0;
            //errs()<<"In function: "<<F.getName()<<"\n";
            for (auto &B : F)  //iterate through all blocks in each function
            { 
                for (auto &I : B) //iterate through all instructions in each block
                { 
                    for (auto op = I.op_begin(); op != I.op_end(); op++) //iterate through all operands in each instruction in the current block
                    {
                        Value* v = op->get(); //get operand Value from pointer op
                        StringRef name = v->getName(); //get name of operand

                        // v->printAsOperand(errs()); //print operand
                        //errs() <<"\n"
                        ;
                        if(name == "AoS_Start")
                        {
                            errs()<<"Array of Structs (AoS) detected"<<"\n";
                            detect = 1;
                        }
                        if(name == "AoS_End")
                        {
                            //errs() <<"End of AoS"<<"\n";
                            detect = 0;
                        }
                    }
                    //print out instructions related to AoS
                    // if(detect == 1)
                    // {
                    //     I.print(errs());
                    //     errs()<<"\n";
                    // }
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
     