# How to create a compiler pass?

1. Create a new folder **/root/passes/newFolderName**, with a new name.

2. In the `CMakeLists.txt` file in **/root/passes** directory, add the following:
``` cmake
# Add new pass directories here:
add_subdirectory(./newFolderName)
```

3. `cd ./newFolder` and create a new `CMakeLists.txt` file.

4. Inside the new file, add the following, replacing `passName`:

``` cmake
add_llvm_pass_plugin(passName
    # List your source files here.
    passName.cpp


    PLUGIN_TOOL
    opt
)
```
5. Now create your .cpp file for the pass and copy this template:

``` cpp
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {

//For Function Pass, use run(Function &F, FunctionAnalysisManager &FM)
struct passName : public PassInfoMixin<passName> {
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
    LLVM_PLUGIN_API_VERSION, "passName", "v0.1",
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, ModulePassManager &MPM, //For FunctionPass use FunctionPassManager &FPM
        ArrayRef<PassBuilder::PipelineElement>) {
          if(Name == "passName"){ //name of pass
            MPM.addPass(printFuncName());
            return true;
          }
          return false;
        }
      );
    }
  };
}
```
6. Return to the folder **/root/passes/** (run `cd ..` from where your pass is located) and run `cmake .`

7. Once successful, run `make` and the "passName.so" file should be generated

8. To use the pass, go to directory of .ll or .bc file and run command:

``` bash
#For example, running pass with name 'printFuncName' in /root/tests/SoA folder

opt -load-pass-plugin=../../passes/sample/printFuncName/printFuncName.so -passes="printFuncName"  < fileToTest.ll > /dev/null
```
`fileToTest.ll` can also be a `.bc` file.

`/dev/null` can be replaced with `fileName.bc` if you want to generate the optimisied/output bitcode file.

9. Run the `clean.sh` bash script in the `/passes` folder to remove the junk files produced by cmake in all subdirectories.

```bash
source clean.sh
```

