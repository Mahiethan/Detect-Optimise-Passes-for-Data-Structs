#!/bin/bash 

clang -emit-llvm unorderedAoS.c -fno-discard-value-names -S -o orderingFields.ll #unoptimised IR
clang -emit-llvm unorderedAoS.c -c -o orderingFields.bc #unoptimised IR bitcode to run

echo "Runtime of unoptimised code (unordered AoS):"
echo ""

/usr/bin/time -v lli orderingFields.bc 2> output.txt
file_content=$(sed -n "5p" "output.txt") 
echo $file_content
echo "Memory usage of unordered AoS:"
file_content=$(sed -n "10p" "output.txt")
echo $file_content
rm output.txt

echo ""
echo "Applying struct field reordering "
echo ""

opt -load-pass-plugin=../../../../passes/optimiseAoS/optimiseAoS.so -passes="optimiseAoS" -time-passes < orderingFields.ll > orderedFields.bc 2> output.txt
file_content=$(sed -n "4p" "output.txt") 
echo $file_content
rm output.txt

echo ""
echo "Runtime of optimised code (ordered AoS):"
echo ""

/usr/bin/time -v lli orderedFields.bc 2> output.txt
file_content=$(sed -n "5p" "output.txt") 
echo $file_content
echo "Memory usage of ordered AoS:"
file_content=$(sed -n "10p" "output.txt")
echo $file_content
rm output.txt
