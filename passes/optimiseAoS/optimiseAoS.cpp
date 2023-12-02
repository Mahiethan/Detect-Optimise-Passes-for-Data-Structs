#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {

//For Function Pass, use run(Function &F, FunctionAnalysisManager &FM)
struct optimiseAoS : public PassInfoMixin<optimiseAoS> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
        //ADD CODE HERE:

        //Sample code to return all function names
        // for (auto &F : M) {
        //     errs() << "I saw a function called " << F.getName() << "!\n";
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
    LLVM_PLUGIN_API_VERSION, "optimiseAoS", "v0.1",
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, ModulePassManager &MPM, //For FunctionPass use FunctionPassManager &FPM
        ArrayRef<PassBuilder::PipelineElement>) {
          if(Name == "optimiseAoS"){ //name of pass
            MPM.addPass(optimiseAoS());
            return true;
          }
          return false;
        }
      );
    }
  };
}