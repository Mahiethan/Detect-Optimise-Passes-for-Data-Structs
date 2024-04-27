#!/bin/bash 

# This just compiles the .c source code files instead of applying the optimisations, like seen with runTetst.sh

echo "First change the size of the AoS data structures (variable SIZE) in unordered.c and ordered.c before running this script."

clang -g unordered.c -o unordered

clang -emit-llvm unordered.c -fno-discard-value-names -S -o unoptimised.ll #produce IR

clang -g ordered.c -o ordered

clang -emit-llvm ordered.c -fno-discard-value-names -S -o ordered.ll






