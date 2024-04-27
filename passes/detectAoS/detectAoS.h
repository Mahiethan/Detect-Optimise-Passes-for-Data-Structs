/*
Header file used for AoS/SoA detection and optimisation
*/
#ifndef DETECTAOS_H
#define DETECTAOS_H
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;
using namespace std;

/* 
Tuple elements of each confirmed AoS:
  1 - AoS
  2 - origin function
  3 - "static" or "dynamic"
  4 -  struct used
  5 - if it used as a function argument - to determine whether struct peeling or splitting should be applied
  6 - if the struct is recursive - to determine whether struct peeling or splitting should be applied
  7 - AoS type in string format - "AoS" or "AoSoA"
*/

vector<tuple<Value*,Function*,string,StructType*,bool,bool,string>> confirmed;


/*
Tuple elements of each confirmed SoA:
  1 - SoA - struct stored as a Value instance
  2 - origin function
  3 - "static" or "dynamic"
  4 - struct used
  5 - if it used as a function argument - to determine whether struct peeling or splitting should be applied
  6 - if the struct is recursive - to determine whether struct peeling or splitting should be applied
*/

vector<tuple<Value*,Function*,string,StructType*,bool,bool>> confirmedSoA;

// used by 'reorderAoS' pass to check whether all structs or only AoS should be optimised
bool detectAoSCalled;

// used in 'detectAoS' pass to store the original sizes of the structures belonging to AoS values. 
// Pair of values: original size and new size. These values will be updated whenever the size of the struct changes during optimisation.
map<StructType*,pair<int,int>> origStructSizes;

// used in 'splitAoS' and 'peelAoS' pass where any created cold structs are stored here. This is present to make sure 'reorderAoS' pass does not skip these structs for optimisation.
vector<StructType*> coldStructs; 

// used by 'detectSoA' to store all SoAs and its sizes
map<Type*,int> toFind;

// used by 'detectSoA' to store any 'potential' AoSoA values that needs to be confirmed by 'detectAoS' pass - NO LONGER USED
vector<tuple<Value*,Function*,string,StructType*>> AoSoAList;

// this boolean is set to false in 'detectAoS' if the source file contains a flag (global variable) that indicates whether struct splitting has already been applied.
// this is then used in 'splitAoS' pass to ensure that struct splitting is not applied twice over an optimised struct.
bool permitStructSplitting;

#endif