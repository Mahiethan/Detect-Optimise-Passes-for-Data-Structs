#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {

struct printFBI : public PassInfoMixin<printFBI> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {

        for (auto &F : M.functions()) //iterate through all functions in the Module and print their names
        { 
            errs() << "In a function called " << F.getName() << "!\n";
            errs()<<"\n"<<"\n";

            //print out function body
            errs() << "Function body:\n";
            F.print(errs());
            errs()<<"\n"<<"\n";

            for (auto &B : F)  //iterate through all blocks in each function
            { 
                errs() << "Basic block:\n";
                B.print(errs());
                errs()<<"\n"<<"\n";

                for (auto &I : B) //iterate through all instructions in each block
                { 
                    errs() << "Instruction: \n";
                    I.print(errs());
                    errs()<<"\n"<<"\n";
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
          if(Name == "printFBI"){ //name of pass
            MPM.addPass(printFBI());
            return true;
          }
          return false;
        }
      );
    }
  };
}