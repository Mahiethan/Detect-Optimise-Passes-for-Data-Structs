#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include "./detectAoS/detectAoS.cpp"
#include "./optimiseAoS/reorderAoS/reorderAoS.cpp"
#include "./optimiseAoS/peelAoS/peelAoS.cpp"
#include "./optimiseAoS/splitAoS/splitAoS.cpp"

using namespace llvm;

//Creates plugin for pass - required
extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "allPasses", "v0.1",
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
          if(Name == "reorderStructs"){ //struct field reordering of all structs
            MPM.addPass(reorderAoS());
            return true;
          }
          if(Name == "peelAoS"){ //struct peeling of AoS data structures
            MPM.addPass(detectAoS());
            MPM.addPass(peelAoS());
            return true;
          }
          if(Name == "peel+reorderAoS"){ //detect, peel and reorder AoS structs 
            MPM.addPass(detectAoS());
            MPM.addPass(peelAoS());
            MPM.addPass(reorderAoS());
            return true;
          }
          if(Name == "reorder+peelAoS"){ //detect, reorder and peel AoS structs - may need reordering again after struct peel
            MPM.addPass(detectAoS());
            MPM.addPass(reorderAoS());
            MPM.addPass(peelAoS());
            return true;
          }
          if(Name == "splitAoS"){ //detect, reorder and peel AoS structs - may need reordering again after struct peel
            MPM.addPass(detectAoS());
            MPM.addPass(splitAoS());
            return true;
          }
          // if(Name == "splitStructs")
          // {
          //   MPM.addPass(splitAoS());
          //   return true;  
          // }
          if(Name == "peel+splitAoS"){ //detect, reorder and peel AoS structs - may need reordering again after struct peel
            MPM.addPass(detectAoS());
            MPM.addPass(peelAoS());
            MPM.addPass(splitAoS());
            return true;
          }
          return false;
        }
      );
    }
  };
}