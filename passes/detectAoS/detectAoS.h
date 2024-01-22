//Header file of detectAoS pass - for use in other passes???
#ifndef DETECTAOS_H
#define DETECTAOS_H
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

// #include <algorithm>
// #include <string>
// #include <deque>

using namespace llvm;
using namespace std;


// vector<tuple<Value*,Function*,string>> potential;
// vector<Value*> argStores;
// vector<pair<Value*,Function*>> possibleGlobals;
vector<tuple<Value*,Function*,string,StructType*,bool>> confirmed;
// vector<tuple<string,vector<int>,Value*>> calledFunction; //stores pair of function name and used argument index of pointer (if any)

// Function* originFunction = NULL;

// int staticCount = 0;
// int dynamicCount = 0;
// bool mallocFlag = false;

// void detectStaticAoS(AllocaInst* AI);

// void eraseFromConfirmed(Value* val);

// namespace llvm {

// struct detectAoS : public PassInfoMixin<detectAoS>{
//     public:
//     PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM);
// };
// }


#endif