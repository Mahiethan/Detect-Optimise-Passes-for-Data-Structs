#!/bin/bash 

clang -emit-llvm unorderedAoS.c -fno-discard-value-names -S -o orderingFields.ll #unoptimised IR
clang -emit-llvm unorderedAoS.c -fno-discard-value-names -c -o orderingFields.bc #unoptimised IR bitcode to run
clang -g unorderedAoS.c -o unorderedAoS

echo "Runtime of unoptimised code (unordered AoS):"
echo ""

/usr/bin/time -v ./unorderedAoS >> output.txt 2>&1
value_a_unordered=$(sed -n "2p" "output.txt") 
echo $value_a_unordered
value_b_unordered=$(sed -n "4p" "output.txt") 
echo $value_b_unordered
value_c_unordered=$(sed -n "6p" "output.txt") 
echo $value_c_unordered
value_d_unordered=$(sed -n "8p" "output.txt") 
echo $value_d_unordered
echo ""
file_content=$(sed -n "14p" "output.txt") 
echo $file_content
echo ""
echo "Memory usage of unordered AoS:"
file_content=$(sed -n "19p" "output.txt")
echo $file_content
rm output.txt
echo ""

echo "Applying struct field reordering "
echo ""

opt -load-pass-plugin=../../../../passes/optimiseAoS/optimiseAoS.so -passes="optimiseAoS" -time-passes < orderingFields.ll > orderedFields.bc 2> output.txt
file_content=$(sed -n "1p" "output.txt") 
echo $file_content
echo ""
file_content=$(sed -n "5p" "output.txt") #originally "4p" without any errs() in optimisation pass
echo $file_content
# # following is used when errs() is used in optimisation pass
# file_content=$(sed -n "1p" "output.txt")
# echo $file_content
# file_content=$(sed -n "2p" "output.txt")
# echo $file_content
# file_content=$(sed -n "3p" "output.txt")
# echo $file_content
# file_content=$(sed -n "7p" "output.txt")
# echo $file_content
rm output.txt

echo ""

echo "Compiling optimised LLVM code to an executable"

llvm-dis orderedFields.bc -o orderedFields.ll > /dev/null 2>&1
llc orderedFields.bc -o orderedFields.s > /dev/null 2>&1 #compiling to native assembly using the LLC code generator
gcc orderedFields.s -o orderedFields.native > /dev/null 2>&1 #Assemble the native assembly language file into a program

rm orderedFields.s

echo ""
echo "Runtime of optimised code (ordered AoS):"
echo ""

/usr/bin/time -v ./orderedFields.native >> output.txt 2>&1
value_a_ordered=$(sed -n "2p" "output.txt") 
echo $value_a_ordered
value_b_ordered=$(sed -n "4p" "output.txt") 
echo $value_b_ordered
value_c_ordered=$(sed -n "6p" "output.txt") 
echo $value_c_ordered
value_d_ordered=$(sed -n "8p" "output.txt") 
echo $value_d_ordered
echo ""
file_content=$(sed -n "14p" "output.txt") 
echo $file_content
echo ""
echo "Memory usage of unordered AoS:"
file_content=$(sed -n "19p" "output.txt")
echo $file_content
rm output.txt

# echo "Compiling unorderedAoS.c with optimisation pass 'optimiseAoS' and running optimised executable 'orderedFields"
# echo ""

# clang -fpass-plugin=`echo ../../../../passes/optimiseAoS/optimiseAoS.so unorderedAoS.c` -o orderedFields
# /usr/bin/time -v ./orderedFields >> output.txt 2>&1
# value_a_ordered=$(sed -n "2p" "output.txt") 
# echo $value_a_ordered
# value_b_ordered=$(sed -n "4p" "output.txt") 
# echo $value_b_ordered
# value_c_ordered=$(sed -n "6p" "output.txt") 
# echo $value_c_ordered
# value_d_ordered=$(sed -n "8p" "output.txt") 
# echo $value_d_ordered
# echo ""
# file_content=$(sed -n "14p" "output.txt") 
# echo $file_content
# echo "Memory usage of ordered AoS:"
# file_content=$(sed -n "19p" "output.txt")
# echo $file_content
# rm output.txt

echo ""

if [ "$value_a_unordered" == "$value_a_ordered" ] && [ "$value_b_unordered" == "$value_b_ordered" ] && [ "$value_c_unordered" == "$value_c_ordered" ] && [ "$value_d_unordered" == "$value_d_ordered" ] 
then
  echo "Outputs after optimisation are correct"
else
  echo "Outputs after optimisation are NOT correct"
fi