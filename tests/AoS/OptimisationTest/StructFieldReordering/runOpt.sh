#!/bin/bash 

clang -emit-llvm unorderedAoS.c -fno-discard-value-names -S -o orderingFields.ll #unoptimised IR
clang -emit-llvm unorderedAoS.c -c -o orderingFields.bc #unoptimised IR bitcode to run

echo "Runtime of unoptimised code (unordered AoS):"
echo ""

/usr/bin/time -v lli orderingFields.bc >> output.txt 2>&1
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
echo "Memory usage of unordered AoS:"
file_content=$(sed -n "19p" "output.txt")
echo $file_content
rm output.txt
echo ""

# echo "Applying struct field reordering "
# echo ""

# opt -load-pass-plugin=../../../../passes/optimiseAoS/optimiseAoS.so -passes="optimiseAoS" -time-passes < orderingFields.ll > orderedFields.bc 2> output.txt
# # file_content=$(sed -n "4p" "output.txt") #originally "4p" without any errs() in optimisation pass
# # echo $file_content
# # following is used when errs() is used in optimisation pass
# file_content=$(sed -n "1p" "output.txt")
# echo $file_content
# file_content=$(sed -n "2p" "output.txt")
# echo $file_content
# file_content=$(sed -n "3p" "output.txt")
# echo $file_content
# file_content=$(sed -n "7p" "output.txt")
# echo $file_content
# rm output.txt

# llvm-dis orderedFields.bc -o orderedFields.ll

# echo ""
# echo "Runtime of optimised code (ordered AoS):"
# echo ""

# /usr/bin/time -v lli orderedFields.bc >> output.txt 2>&1
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
# echo "Memory usage of unordered AoS:"
# file_content=$(sed -n "19p" "output.txt")
# echo $file_content
# rm output.txt

# echo ""
# if [ "$value_a_unordered" == "$value_a_ordered" ] && [ "$value_b_unordered" == "$value_b_ordered" ] && [ "$value_c_unordered" == "$value_c_ordered" ] && [ "$value_d_unordered" == "$value_d_ordered" ] 
# then
#   echo "Outputs after optimisation are correct"
# fi
# echo ""

echo "Compiling unorderedAoS.c with optimisation pass 'optimiseAoS' and running optimised executable 'orderedFields"
echo ""

clang -fpass-plugin=../../../../passes/optimiseAoS/optimiseAoS.so unorderedAoS.c -o orderedFields
./orderedFields >> output.txt
value_a_ordered=$(sed -n "2p" "output.txt") 
echo $value_a_ordered
value_b_ordered=$(sed -n "4p" "output.txt") 
echo $value_b_ordered
value_c_ordered=$(sed -n "6p" "output.txt") 
echo $value_c_ordered
value_d_ordered=$(sed -n "8p" "output.txt") 
echo $value_d_ordered
rm output.txt

echo ""

if [ "$value_a_unordered" == "$value_a_ordered" ] && [ "$value_b_unordered" == "$value_b_ordered" ] && [ "$value_c_unordered" == "$value_c_ordered" ] && [ "$value_d_unordered" == "$value_d_ordered" ] 
then
  echo "Outputs after optimisation are correct"
fi