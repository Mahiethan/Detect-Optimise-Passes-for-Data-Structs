#!/bin/bash 

clang -g unsplit.c -o unsplit

############################################ Measuring compiling time ########################################

#Capturing start time in milliseconds
start_time=$(date +%s%3N)

clang -emit-llvm unsplit.c -fno-discard-value-names -S -o unoptimised.ll #produce IR

opt -load-pass-plugin=../DCSBatch/allPasses.so -passes="split+reorderAoS" -time-passes < unoptimised.ll > optimised.bc #apply compiler passes

clang -g optimised.bc -o optimised # convert LLVM bitcode to machine code executable

#Capturing end time in milliseconds
end_time=$(date +%s%3N)

#############################################################################################################

llvm-dis optimised.bc # produce IR of optimised program (for inspection)

#Calculating elapsed time in milliseconds
milli_time=$((end_time - start_time))

#Displaying elapsed time in milliseconds
echo "Elapsed time (milliseconds): $milli_time ms"
