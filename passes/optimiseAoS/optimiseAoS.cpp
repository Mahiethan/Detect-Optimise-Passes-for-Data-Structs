#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LLVMContext.h"

#include <string>

using namespace llvm;
using namespace std;

LLVMContext TheContext;
IRBuilder<> Builder(TheContext);

ArrayRef<Type*> elemArr;
vector<tuple<Type*,int,int>> sortedElems;

namespace {

struct optimiseAoS : public PassInfoMixin<optimiseAoS> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
        vector<StructType*> allStructs = M.getIdentifiedStructTypes();

        for(int i = 0; i < allStructs.size(); i++)
        {
          //get element type array
          elemArr = allStructs.at(i)->elements();

          vector<tuple<Type*,int,int>> elems;
        
          int o = 0;
          for(auto it = elemArr.begin(); it != elemArr.end(); it++)
          {
            Type* ty = const_cast<Type*>(*it);

            elems.push_back(make_tuple(ty,o,0));
            o++;
          }

          //sort by decreasing type size
          bool none = false;
          int f = 0;
          while(none == false)
          {
            none = true;
            for(auto it = elems.begin(); it != elems.end(); it)
            {
              Type* ty = get<0>(*it);
              int o = get<1>(*it);
              if(ty->isDoubleTy())
              {
                sortedElems.push_back(make_tuple(ty,o,f));
                elems.erase(it);
                f++;
                none = false;
              }
              else
                it++;
            }
          }

          none = false;
          while(none == false)
          {
            none = true;
            for(auto it = elems.begin(); it != elems.end(); it)
            {
              Type* ty = get<0>(*it);
              int o = get<1>(*it);
              if(ty->isIntegerTy(32))
              {
                sortedElems.push_back(make_tuple(ty,o,f));
                elems.erase(it);
                f++;
                none = false;
              }
              else
                it++;
            }
          }

          none = false;
          while(none == false)
          {
            none = true;
            for(auto it = elems.begin(); it != elems.end(); it)
            {
              Type* ty = get<0>(*it);
              int o = get<1>(*it);
              if(ty->isIntegerTy(8))
              {
                sortedElems.push_back(make_tuple(ty,o,f));
                elems.erase(it);
                f++;
                none = false;
              }
              else
                it++;
            }
          }

          vector<Type*> newElems;

          for(int i = 0; i < sortedElems.size(); i++)
            newElems.push_back(get<0>(sortedElems.at(i)));
          
          ArrayRef<Type*> elemArr = ArrayRef(newElems);

          allStructs.at(i)->setBody(elemArr, /* packed */ false);

          }

        for (auto &F : M) //iterate through all functions in the Module and print their names
        { 
          for (auto &B : F)  //iterate through all blocks in each function
          { 
              for (auto &I : B) //iterate through all instructions in each block
              {
                 if(auto *GEP = dyn_cast<GetElementPtrInst>(&I))
                 {
                  Value* ret = GEP->getOperand(GEP->getNumIndices()); //getting last operand
                  string op_str; 
                  raw_string_ostream to_str(op_str);
                  ret->printAsOperand(to_str);

                  if((op_str.find('%') == string::npos) & (!(GEP->getResultElementType()->isStructTy())))
                  {
                    size_t space_pos = op_str.find(" ");    
                    string type;
                    string indexStr;
                    //string manipulation to get 'type' and 'name'
                    if (space_pos != std::string::npos) 
                    {
                      type = op_str.substr(0, space_pos);
                    }

                    indexStr = op_str.substr(space_pos + 1);

                    int index;

                    if(indexStr.find("%") == string::npos)
                      index = std::stoi(indexStr);


                    for(int i = 0; i < sortedElems.size(); i++)
                    {
                      int currIndex = get<1>(sortedElems.at(i));
                      int newIndex = get<2>(sortedElems.at(i));

                      if(index == currIndex)
                      {
                        index = newIndex;
                        break;
                      }

                    }
                    GEP->setOperand(GEP->getNumIndices(),ConstantInt::get(TheContext,APInt(32,index)));
                  }
                 }
              }
          }
        }

        //Set to ::all() if IR is unchanged, otherwise ::none()
        return PreservedAnalyses::none();
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