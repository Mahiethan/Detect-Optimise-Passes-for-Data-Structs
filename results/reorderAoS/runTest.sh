#!/bin/bash 

echo "First change the size of the AoS data structures (variable n) in unordered.c and ordered.c before running this script."

clang -g unordered.c -o unordered

clang -emit-llvm unordered.c -fno-discard-value-names -S -o unoptimised.ll

clang -g ordered.c -o ordered

clang -emit-llvm ordered.c -fno-discard-value-names -S -o ordered.ll

opt -load-pass-plugin=allPasses.so -passes="reorderAoS" -time-passes < unoptimised.ll > optimised.bc && llvm-dis optimised.bc && clang -g optimised.bc -o optimised

# Uncomment below if you want to run it on current terminal - does not give reliable results

# /usr/bin/time -v ./unordered >> unorderedOutput.txt 2>&1 | tee unorderedOutput.txt

# /usr/bin/time -v ./ordered >> orderedOutput.txt 2>&1 | tee orderedOutput.txt

# /usr/bin/time -v ./optimised >> optimisedOutput.txt 2>&1 | tee optimisedOutput.txt


