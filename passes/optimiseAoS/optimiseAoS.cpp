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

ArrayRef<Type*> elemArr; //original struct fields

vector<tuple<Type*,int,int,int>> sortedElems; //sorted struct fields

string modifiedStruct; //name of modified struct

bool performOpt = false;

namespace {

struct optimiseAoS : public PassInfoMixin<optimiseAoS> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {

        /// this is to be replaced with structs used in AoS 
        vector<StructType*> allStructs = M.getIdentifiedStructTypes(); //get all struct types used in program

        for(int i = 0; i < allStructs.size(); i++)
        {
          //get element type array
          elemArr = allStructs.at(i)->elements();

          vector<tuple<Type*,int,int,int>> elems; //the type, original index, final index and size of each field will be kept track
        
          DataLayout* TD = new DataLayout(&M);

          int o = 0;
          int oldStructSize = 0; //in bytes
          int wordSize = 0;
          int currWord = 0; 

          /// get current struct fields and calculate the size of struct
          /// old index position of field is stored
          for(auto it = elemArr.begin(); it != elemArr.end(); it++)
          {
            Type* ty = const_cast<Type*>(*it);
            int size = TD->getTypeAllocSize(ty);
            if(currWord + size <= 8) 
              currWord = currWord + size;
            else
            {
              oldStructSize += 8;
              if(size >= 8)
              {
                currWord = 0;
                oldStructSize += 8;
              }
              else
              {
                currWord = size;
              }
            }
            elems.push_back(make_tuple(ty,o,0,size));
            o++;
          }

          if(currWord > 0)
            oldStructSize += 8;
          
          /* 
             sort fields by decreasing type size
             new index position of field is stored 
          */
          bool none = false;
          int f = 0;
          while(none == false)
          {
            none = true;
            int MAX = INT_MIN;
            int o; //old index
            vector<std::tuple<llvm::Type *, int, int, int>>::iterator toDelete;
            for(auto it = elems.begin(); it != elems.end(); it++)
            {
              Type* ty = get<0>(*it);
              int size = get<3>(*it);
              if(size > MAX)
              {
                toDelete = it;
                MAX = size;
                none = false;
              }
            }

            if(elems.size() == 0)
            {
              none = true;
            }
            else
            {
              sortedElems.push_back(make_tuple(get<0>(*toDelete),get<1>(*toDelete),f,get<3>(*toDelete)));
              f++;
              elems.erase(toDelete);
            }
          }


          /* calculate size of new struct */
          vector<Type*> newElems;
          int newStructSize = 0; //in bytes
          wordSize = 0;
          currWord = 0;
          for(int i = 0; i < sortedElems.size(); i++)
          {
            int typeSize = get<3>(sortedElems.at(i));
            if(currWord + typeSize <= 8) 
              currWord = currWord + typeSize;
            else
            {
              newStructSize += 8;
              if(typeSize >= 8)
              {
                currWord = 0;
                newStructSize += 8;
              }
              else
              {
                currWord = typeSize;
              }
            }

            newElems.push_back(get<0>(sortedElems.at(i)));
          }

          if(currWord > 0)
            newStructSize += 8;


          ArrayRef<Type*> elemArr = ArrayRef(newElems);

          /* if new struct size is smaller, perform optimisation, otherwise skip */
          if(newStructSize < oldStructSize)
          {
            errs()<<"Performing optimisation. Saving "<<oldStructSize-newStructSize<<" bytes in struct size.\n";
            performOpt = true;
            allStructs.at(i)->setBody(elemArr, false);

            raw_string_ostream t_str(modifiedStruct);
            allStructs.at(i)->print(t_str);
          }
          else
          {
            errs()<<"Not performing optimisation. ";
            if(newStructSize > oldStructSize)
              errs()<<"New size is larger than old struct by "<<newStructSize-oldStructSize<<" bytes.\n";
            else
              errs()<<"New struct has same size as old struct: "<<newStructSize<<" bytes.\n";
          }


          bool replaceConstExpr = false;
          ConstantExpr* ce;

          /// changing indices of all GEP instructions that use the modified struct
          if(performOpt == true)
          {
            for (auto &F : M) 
            { 
              for (auto &B : F)  
              { 
                  bool foundAoS = false;
                  for (auto &I : B) /// iterate through all instructions
                  {
                    if(auto *GEP = dyn_cast<GetElementPtrInst>(&I)) //find a GEP instruction
                    {
                      Value* ret = GEP->getOperand(GEP->getNumIndices()); //getting last operand
                      string op_str; 
                      raw_string_ostream to_str(op_str);
                      ret->printAsOperand(to_str);

                      Type* structUsed = GEP->getSourceElementType(); //get the struct ptr that is being accessed
                      string ret_str; 
                      raw_string_ostream t_str(ret_str);
                      structUsed->print(t_str);

                      /* An access to an AoS elements involves two GEP instructions:
                        - First to access the struct pointer - source element type - type struct
                        - Second to access field of the struct pointer - source element type - same as the field type
                      */

                      //These two operations should exists within the same block, since the flag is set to false in a new block

                      if(ret_str == modifiedStruct) //if an access to the modified AoS is found, turn flag on to change its GEP operations
                      {
                        foundAoS = true;
                        //continue; //skip the AoS element access, and go to the following GEP which accesses a field of the element
                      }

                      /* Only access/change indicies of GEP instruction if:
                        - it is not equal to a % value - it should only be equal to a i32 value
                        - the result element type is not a struct - this should be identified in the preceding GEP (access to the struct ptr)
                        - if the modified struct is being accessed in the preceding GEP - indicated by this boolean flag
                      */ 

                      if((op_str.find('%') == string::npos) & (!(GEP->getResultElementType()->isStructTy())) & (foundAoS == true)) 
                      {
                        size_t space_pos = op_str.find(" ");    
                        string type;
                        string indexStr;

                        //string manipulation to get 'type' and 'name' of the indices
                        if (space_pos != std::string::npos) 
                        {
                          type = op_str.substr(0, space_pos);
                        }

                        indexStr = op_str.substr(space_pos + 1);

                        int index;

                        if(indexStr.find("%") == string::npos)
                          index = std::stoi(indexStr);

                        /// replace old indices with new index positions (if the field has changed position)
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

                        GEP->setOperand(GEP->getNumIndices(),ConstantInt::get(TheContext,APInt(32,index))); //new index for the GEP instruction is set
                        foundAoS = false;
                      }
                    }
                  }
              }
            }
          }

          /// reset values and arrays to process next AoS struct
          sortedElems.clear();
          performOpt = false;
          modifiedStruct = "";
          
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