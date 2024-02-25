#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include "./helper/removeConstantGEP.cpp"
#include "./detectAoS/detectAoS.cpp"
#include "./optimiseAoS/reorderAoS/reorderAoS.cpp"
#include "./optimiseAoS/peelAoS/peelAoS.cpp"
#include "./optimiseAoS/splitAoS/splitAoS.cpp"
#include "./detectSoA/detectSoA.cpp"


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

          MPM.addPass(removeConstantGEP()); //converts any constant GEP to standalone GEP instructions

          // if(Name == "detectAoS"){ //detectAoS - DEPRECATED - run detectAoSoA instead - it will detect both SoA and AoS separately
          //   MPM.addPass(detectAoS());
          //   return true;
          // }
          if(Name == "reorderAoS"){ //struct field reordering of AoS data structures - not including any cold structs
            MPM.addPass(detectAoS());
            MPM.addPass(reorderAoS());
            return true;
          }
          if(Name == "reorderStructs"){ //struct field reordering of all structs - including used/unused cold structs
            MPM.addPass(reorderAoS());
            return true;
          }
          if(Name == "peelAoS"){ //struct peeling of AoS data structures
            MPM.addPass(detectAoS());
            MPM.addPass(peelAoS());
            return true;
          }
          if(Name == "peel+reorderAoS"){ //detect, peel and reorder AoS structs - including cold structs
            MPM.addPass(detectAoS());
            MPM.addPass(peelAoS());
            MPM.addPass(reorderAoS());
            return true;
          }
          if(Name == "reorder+peelAoS"){ //detect, reorder and peel AoS structs - may need reordering again after struct peel - doesn't optimise used cold structs
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

          if(Name == "reorder+splitAoS"){ //detect, reorder and split AoS structs
            MPM.addPass(detectAoS());
            MPM.addPass(splitAoS());
            MPM.addPass(reorderAoS());
            return true;
          }

          if(Name == "split+reorderAoS"){ //detect, split and reorder AoS structs - including cold structs
            MPM.addPass(detectAoS());
            MPM.addPass(splitAoS());
            MPM.addPass(reorderAoS());
            return true;
          }

          if(Name == "detectSoA"){ //detect SoA data structures
            MPM.addPass(detectSoA());
            return true;
          }

          if(Name == "detectAoSoA"){ //detect SoA, AoS and AoSoA data structures
            MPM.addPass(detectSoA());
            MPM.addPass(detectAoS());
            return true;
          }

          return false;
        }
      );
    }
  };
}