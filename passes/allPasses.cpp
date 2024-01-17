#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include "./detectAoS/detectAoS.cpp"
#include "./optimiseAoS/reorderAoS/reorderAoS.cpp"
// #include "./optimiseAoS/peelAoS/peelAoS.cpp"

using namespace llvm;

//Creates plugin for pass - required
extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "reorderAoS", "v0.1",
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, ModulePassManager &MPM, //For FunctionPass use FunctionPassManager &FPM
        ArrayRef<PassBuilder::PipelineElement>) {
          if(Name == "detectAoS"){ //detectAoS
            MPM.addPass(detectAoS());
            return true;
          }
          if(Name == "reorderAoS"){ //struct field reordering of AoS data structures
            MPM.addPass(detectAoS());
            MPM.addPass(reorderAoS());
            return true;
          }
          if(Name == "reorderAoSOnly"){ //struct field reordering
            MPM.addPass(reorderAoS());
            return true;
          }
        //   if(Name == "reorder+PeelAoS"){ //struct field reordering
        //     // MPM.addPass(detectAoS());
        //     // MPM.addPass(reorderAoS());
        //     // MPM.addPass(peelAoS());
        //     return true;
        //   }
          return false;
        }
      );
    }
  };
}