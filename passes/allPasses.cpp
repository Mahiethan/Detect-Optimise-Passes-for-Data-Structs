#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include "./helper/removeConstantGEP.cpp" // "removeConstantGEP" pass

#include "./detectAoS/detectAoS.cpp" // "detectAoS" pass
#include "./optimiseAoS/reorderAoS/reorderAoS.cpp" // "reorderAoS" pass
#include "./optimiseAoS/peelAoS/peelAoS.cpp" // "peelAoS" pass
#include "./optimiseAoS/splitAoS/splitAoS.cpp" // "splitAoS" pass
#include "./detectSoA/detectSoA.cpp" // "detectSoA" pass

using namespace llvm;

extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "allPasses", "v0.1",
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, ModulePassManager &MPM,
        ArrayRef<PassBuilder::PipelineElement>) {

          // before applying any detection / optimisation passes, remove constant GEP instructions

          MPM.addPass(removeConstantGEP()); //converts any constant GEP to standalone GEP instructions

          // now detection and/or optimisation can begin

          /*        DETECTION ONLY         */

          if(Name == "detectSoA"){ // detect SoA data structures
            MPM.addPass(detectSoA());
            return true;
          }

          if(Name == "detectAoSoA"){ // detect SoA, AoS and AoSoA data structures
            MPM.addPass(detectSoA());
            MPM.addPass(detectAoS());
            return true;
          }

          /*        DETECTION & SINGLE OPTIMISATION         */


          if(Name == "reorderAoS"){ // struct field reordering of AoS data structures
            MPM.addPass(detectAoS());
            MPM.addPass(reorderAoS());
            return true;
          }
          if(Name == "reorderStructs"){ // struct field reordering of all structs
            MPM.addPass(reorderAoS());
            return true;
          }
          if(Name == "peelAoS"){ // struct peeling of AoS data structures
            MPM.addPass(detectAoS());
            MPM.addPass(peelAoS());
            return true;
          }
          if(Name == "splitAoS"){ // structure splitting of AoS data structures
            MPM.addPass(detectAoS());
            MPM.addPass(splitAoS());
            return true;
          }

          /*        DETECTION & MULTIPLE OPTIMISATIONS         */


          if(Name == "peel+reorderAoS"){ // detect, peel and reorder AoS structs
            MPM.addPass(detectAoS());
            MPM.addPass(peelAoS()); // cold structs will be created here, which can have its fields reordered by the next compiler pass
            MPM.addPass(reorderAoS());
            return true;
          }
          if(Name == "split+reorderAoS"){ // detect, split and reorder AoS structs
            MPM.addPass(detectAoS());
            MPM.addPass(splitAoS());
            MPM.addPass(reorderAoS());
            return true;
          }

          /* Following combinations are not as effective as above two, since struct reordering is applied before the cold structs are created, so cold structs are not reordered
          Only present to show that the compiler passes work together in any order, without breaking the program's functionality
          */


          if(Name == "reorder+peelAoS"){ // detect, reorder and peel AoS data structuress
            MPM.addPass(detectAoS());
            MPM.addPass(reorderAoS());
            MPM.addPass(peelAoS());
            return true;
          }

          if(Name == "reorder+splitAoS"){ // detect, reorder and split AoS data structures
            MPM.addPass(detectAoS());
            MPM.addPass(reorderAoS());
            MPM.addPass(splitAoS());
            return true;
          }

          if(Name == "peel+splitAoS"){ // detect, peel and split AoS data structures
            MPM.addPass(detectAoS());
            MPM.addPass(peelAoS());
            MPM.addPass(splitAoS());
            return true;
          }

          if(Name == "split+peelAoS"){ // detect, split and peel AoS data structures
            MPM.addPass(detectAoS());
            MPM.addPass(splitAoS());
            MPM.addPass(peelAoS());
            return true;
          }


          return false; // if incorrect pass name given, return false
        }
      );
    }
  };
}