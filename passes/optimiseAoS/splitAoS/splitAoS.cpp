#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include "../../detectAoS/detectAoS.h"

using namespace llvm;
using namespace std;

LLVMContext split_Context;
IRBuilder<> split_Builder(split_Context);

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

        errs()<<"\n-------------------------- STRUCT SPLITTING --------------------------\n\n";

        vector<StructType*> allStructs; //stores all structs to be optimised
        // vector<pair<GlobalVariable*,StructType*>> globalAoS;
        // vector<tuple<Value*,StructType*,Function*>> localAoS;

        /// ONLY COLLECT STRUCTS OF AoS values that satisfy one or multiple of the following criteria:
        // - are used as function arguments
        // - dynamic
        // - contains a ptr element - which might make it a recursive struct

        if(confirmed.size() == 0) //if no AoS is found, do not apply this optimisation (MAYBE GET RID OF THIS, IT IS POSSIBLE TO SPLIT ALL STRUCTS)
        {
          errs()<<"No AoS values found. Not applying struct peeling.\n";
          errs()<<"\n----------------------- END OF STRUCT PEELING -----------------------\n";
          return PreservedAnalyses::all();
        }
        else
        {
          //get structs used by all AoS
          for(int i = 0; i < confirmed.size(); i++)
          {
            StructType* structure = get<3>(confirmed.at(i));
            Value* aos = get<0>(confirmed.at(i));
            Function* origFunc = get<1>(confirmed.at(i));
            bool isDynamic = false;
            if(get<2>(confirmed.at(i)) == "dynamic")
              isDynamic = true;
            bool isParam = get<4>(confirmed.at(i));
            bool hasPointerElem = get<5>(confirmed.at(i));

            DataLayout* TD = new DataLayout(&M);

            if((isParam | hasPointerElem | isDynamic) & (TD->getStructLayout(structure)->getSizeInBytes() > 8)) //only optimise structs that have a size greater than a word length (8 bytes)
            {
              if(structure == nullptr) //should not occur
              {
                errs()<<"WARNING: nullptr found!\n";
                continue;
              }
              bool exist = false;
              for(int j = 0; j < allStructs.size(); j++)
              {
                if(allStructs.at(j) == structure)
                {
                  exist = true;
                  break;
                }
              }
              if(exist == false)
              {
                allStructs.push_back(structure);
                errs()<<"Optimising "<<structure->getName()<<"\n";
              }
              else
              {
                errs()<<"Dupe found\n";
              }
              
              // if(auto *GV = dyn_cast<GlobalVariable>(aos))
              // {
              //   globalAoS.push_back(make_pair(GV,structure));
              // }
              // else
              // {
              //   localAoS.push_back(make_tuple(aos,structure,origFunc));
              // }
            }
            else
            {
              //erase the structure from globalAoS and localAoS vectors - cannot optimise it - used as function parameter
              // for(auto it = globalAoS.begin(); it != globalAoS.end(); it)
              // {
              //   StructType* structToRemove = get<1>(*it);
              //   if(structToRemove == structure)
              //   {
              //     globalAoS.erase(it);
              //   }
              //   else
              //   {
              //     it++;
              //   }
              // }

              // for(auto it = localAoS.begin(); it != localAoS.end(); it)
              // {
              //   StructType* structToRemove = get<1>(*it);
              //   if(structToRemove == structure)
              //   {
              //     localAoS.erase(it);
              //   }
              //   else
              //   {
              //     it++;
              //   }
              // }

              for(auto it = allStructs.begin(); it != allStructs.end(); it)
              {
                StructType* structToRemove = *it;
                if(structToRemove == structure)
                {
                  errs()<<"No longer optimising "<<structure->getName()<<"\n";
                  allStructs.erase(it);
                }
                else
                {
                  it++;
                }
              }
            }
          }
        }

        if(allStructs.size() != 0) 
        {
          // errs()<<"Optimising "<<globalAoS.size()<<" global AoS and "<<localAoS.size()<<" local AoS, which contain the following struct(s):\n";
          errs()<<"Optimising the following struct(s):\n";
        }
        else
        {
          errs()<<"No structs available to optimise. Not applying struct peeling.\n";
          errs()<<"\n----------------------- END OF STRUCT PEELING -----------------------\n";
          return PreservedAnalyses::all(); 
        }
        

        for(int i = 0; i < allStructs.size(); i++)
        {
          allStructs.at(i)->print(errs());
          errs()<<"\n";
          
          StructType* currStruct = allStructs.at(i);

          vector<float> elems; //this is used to store the no. of times each field element was used in the current struct

          for(int j = 0; j < currStruct->elements().size(); j++) //initialise counts to zero
            elems.push_back(0.0);

          // for each instruction, look for GEP instructions and count the number of times each field of a struct was used
          for (auto &F : M) 
          { 
            for (auto &B : F)  
            {
              // LoopInfo& loops = getAnalysis<LoopInfoWrapperPass>().getLoopInfo(); //sort this out ltr - check trello

              //// SORT OUT LATER ////
              // LoopInfo LI = LoopInfo();
              // Loop* l = LI.getLoopFor(&B); 
              // runOnLoop(l); //gives seg fault

              for (auto &I : B) /// iterate through all instructions
              {
                if(auto *GEP = dyn_cast<GetElementPtrInst>(&I)) //for GEP instructions
                {
                  // only inspect GEP instructions that access the current struct, but does not return the current struct
                  if(GEP->getSourceElementType() == currStruct & GEP->getResultElementType() != currStruct)
                  {
                    string operand_str;
                    raw_string_ostream str(operand_str);
                    Value* index = GEP->getOperand(GEP->getNumIndices()); //get last index value being access
                    index->printAsOperand(str);

                    size_t space_pos = operand_str.find(" ");    
                    string type;
                    string indexStr;

                    // string manipulation to get 'type' and 'index' of the indices
                    if (space_pos != std::string::npos) 
                    {
                      type = operand_str.substr(0, space_pos);
                    }

                    indexStr = operand_str.substr(space_pos + 1);

                    int in = std::stoi(indexStr); // index accessed by GEP instruction

                    /// increment correct index counter 
                    float count = elems.at(in);
                    count++;
                    elems.at(in) = count;
                  }
                }
              }
          }
        }

        /// finding mean no. of accesses
        float total = 0.0;
        float mean = 0.0;
        for(int j = 0; j < elems.size(); j++)
        {
          // errs()<<elems.at(j)<<"\n";
          total += elems.at(j);  
        }

        mean = total/elems.size();

        // split fields based on mean no. of accesses
        // with no. of accesses >= mean, put field in hot struct
        // with no. of accesses < mean, put field in cold struct

        ArrayRef<Type*> elemTypes = currStruct->elements(); // get all element types of current struct
        
        /// vectors to store fields and indices for hot and cold structs
        vector<Type*> hotFields;
        vector<pair<int,int>> hotIndices;
        vector<Type*> coldFields;
        vector<pair<int,int>> coldIndices;

        /// put each struct field into a hot or cold struct, based on whether its been accessed frequently (> mean)
        int index = 0;
        int coldIndex = 0;
        int hotIndex = 0;
        for(auto it = elemTypes.begin(); it != elemTypes.end(); it++)
        {
          if(elems.at(index) >= mean)
          {
            hotFields.push_back(*it);
            hotIndices.push_back(make_pair(index,hotIndex));
            hotIndex++;
          }
          else if(elems.at(index) < mean)
          {
            coldFields.push_back(*it);
            coldIndices.push_back(make_pair(index,coldIndex));
            coldIndex++;
          }

          index++;
        }

        if(coldFields.size() == 0) //no cold field required for this struct
        {
          errs()<<"No need to peel this struct - no cold fields found\n";
          break;
        }

        //// create new hot and cold structs
        //// for hot struct, replace fields of current struct, keeping the same struct name, and add a new ptr field to the cold struct
        //// for cold struct, create new struct with the cold fields, with new struct name

        ////// ADD POINTER TO COLD STRUCT WITHIN HOT STRUCT

        ArrayRef<Type*> newColdFields = ArrayRef(coldFields);
        
        // //// creating hot struct
        // allStructs.at(i)->setBody(newHotFields);
        
        //// creating cold struct
        string coldName = currStruct->getName().str();
        coldName = coldName.append("Cold");
        StructType* coldStruct = StructType::create(split_Context, coldName);
        coldStruct->setBody(coldFields);

        PointerType* coldStructPtr = PointerType::get(coldStruct, 0); // Initialise the pointer type now

        // //// creating hot struct
        hotFields.push_back(coldStructPtr);
        ArrayRef<Type*> newHotFields = ArrayRef(hotFields);
        allStructs.at(i)->setBody(newHotFields);

        errs()<<"Hot Struct:\n";
        currStruct->print(errs());
        errs()<<"\nCold Struct:\n";
        coldStruct->print(errs());
        errs()<<"\n";

        // //change GEP

        //when an access to a field that now belongs to the cold struct is found within a block, perform (in this order):
        //1. create GEP inst to access the cold struct ptr
        //2. create new malloc call inst to allocate data
        //3. create store inst to store the returned malloc memory to the GEP 
        //4. create a load inst to the GEP as a ptr, so the pointer operand and the indices of existing GEP inst can be changed for the cold fields

        }

        errs()<<"\n----------------------- END OF STRUCT SPLITTING -----------------------\n";


        //Set to ::all() if IR is unchanged, otherwise ::none()
        return PreservedAnalyses::none();
    };
};

}


// //Creates plugin for pass - required
// extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
// llvmGetPassPluginInfo() {
//   return {
//     LLVM_PLUGIN_API_VERSION, "splitAoS", "v0.1",
//     [](PassBuilder &PB) {
//       PB.registerPipelineParsingCallback(
//         [](StringRef Name, ModulePassManager &MPM, //For FunctionPass use FunctionPassManager &FPM
//         ArrayRef<PassBuilder::PipelineElement>) {
//           if(Name == "splitAoS"){ //name of pass
//             MPM.addPass(splitAoS());
//             return true;
//           }
//           return false;
//         }
//       );
//     }
//   };
// }