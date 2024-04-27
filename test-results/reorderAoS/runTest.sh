#!/bin/bash 

clang -g unordered.c -o unordered

clang -g ordered.c -o ordered

clang -emit-llvm ordered.c -fno-discard-value-names -S -o ordered.ll

############################################ Measuring compiling time ########################################

#Capturing start time in milliseconds
start_time=$(date +%s%3N)

clang -emit-llvm unordered.c -fno-discard-value-names -S -o unoptimised.ll #produce IR

opt -load-pass-plugin=../DCSBatch/allPasses.so -passes="reorderAoS" -time-passes < unoptimised.ll > optimised.bc #apply compiler passes

clang -g optimised.bc -o optimised # convert LLVM bitcode to machine code executable

#Capturing end time in milliseconds
end_time=$(date +%s%3N)

#############################################################################################################

llvm-dis optimised.bc # produce IR of optimised program (for inspection)

#Calculating elapsed time in milliseconds
milli_time=$((end_time - start_time))

#Displaying elapsed time in milliseconds
echo "Elapsed time (milliseconds): $milli_time ms"

# Uncomment below if you want to run it on current terminal - does not give reliable results

# /usr/bin/time -v ./unordered >> unorderedOutput.txt 2>&1 | tee unorderedOutput.txt

# /usr/bin/time -v ./ordered >> orderedOutput.txt 2>&1 | tee orderedOutput.txt

# /usr/bin/time -v ./optimised >> optimisedOutput.txt 2>&1 | tee optimisedOutput.txt