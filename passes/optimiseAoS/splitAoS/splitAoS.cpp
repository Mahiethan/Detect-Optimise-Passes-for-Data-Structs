#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {

//For Function Pass, use run(Function &F, FunctionAnalysisManager &FM)
struct splitAoS : public PassInfoMixin<splitAoS> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
        //ADD CODE HERE:

        // get vector of <AoS>
        // for each struct
            // find AoS that use current struct and put it in new list
        // if this list size > 0, proceed with struct splitting (use code from peelAoS)
            //create new cold struct
            //make a new struct ptr field in the current struct that points to the cold struct
            //calculate new indices
        // otherwise skip to next struct

            // for each AoS in list
                //iterate through all GEP
                //if GEP accesses a global (first operand) and source == curr struct and return == curr struct
                    //set prec to true
                //if prec is true and source == curr struct and return != curr struct
                    //determine whether a cold or hot index was used and make aprropriate changes (use existing code)
                    //for cold indices, need to add a new GEP to cold struct 
                    //create new load to that ptr, and access new index


        //Sample code to return all function names
        // for (auto &F : M) {
        //     errs() << "I saw a function called " << F.getName() << "!\n";
        // }

        //when an access to a field that now belongs to the cold struct is found within a block, perform (in this order):
          //1. create GEP inst to access the cold struct ptr
          //2. create new malloc call inst to allocate data
          //3. create store inst to store the returned malloc memory to the GEP 
          //4. create a load inst to the GEP as a ptr, so the pointer operand and the indices of existing GEP inst can be changed for the cold fields

        //Set to ::all() if IR is unchanged, otherwise ::none()
        return PreservedAnalyses::all();
    };
};

}


//Creates plugin for pass - required
extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "splitAoS", "v0.1",
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, ModulePassManager &MPM, //For FunctionPass use FunctionPassManager &FPM
        ArrayRef<PassBuilder::PipelineElement>) {
          if(Name == "splitAoS"){ //name of pass
            MPM.addPass(splitAoS());
            return true;
          }
          return false;
        }
      );
    }
  };
}