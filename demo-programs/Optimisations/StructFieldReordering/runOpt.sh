#!/bin/bash 

echo "-----------------------------------------------------------"
echo "Running test one"
echo ""

clang -emit-llvm unorderedAoS.c -fno-discard-value-names -S -o orderingFields.ll #unoptimised IR
clang -emit-llvm unorderedAoS.c -fno-discard-value-names -c -o orderingFields.bc #unoptimised IR bitcode to run
clang -g unorderedAoS.c -o unorderedAoS

echo "Runtime of unoptimised code (unordered AoS):"
echo ""

/usr/bin/time -v lli orderingFields.ll >> output.txt 2>&1
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

opt -load-pass-plugin=../../../passes/allPasses.so -passes="reorderAoS" -time-passes < orderingFields.ll > orderedFields.bc 2> output.txt
file_content=$(sed -n "1p" "output.txt") 
echo $file_content
echo ""
file_content=$(sed -n "5p" "output.txt") #originally "4p" without any errs() in optimisation pass
echo $file_content
rm output.txt

echo ""

echo "Compiling optimised LLVM code to an executable"

llvm-dis orderedFields.bc -o orderedFields.ll > /dev/null 2>&1

echo ""
echo "Runtime of optimised code (ordered AoS):"
echo ""

/usr/bin/time -v lli orderedFields.ll >> output.txt 2>&1
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
echo "Memory usage of ordered AoS:"
file_content=$(sed -n "19p" "output.txt")
echo $file_content
rm output.txt

echo ""

if [ "$value_a_unordered" == "$value_a_ordered" ] && [ "$value_b_unordered" == "$value_b_ordered" ] && [ "$value_c_unordered" == "$value_c_ordered" ] && [ "$value_d_unordered" == "$value_d_ordered" ] 
then
  echo "Outputs after optimisation are correct"
else
  echo "Outputs after optimisation are NOT correct"
fi

echo "-----------------------------------------------------------"
echo "Running test two"
echo ""

clang -emit-llvm unorderedTwo.c -fno-discard-value-names -S -o orderingTwo.ll #unoptimised IR
clang -emit-llvm unorderedTwo.c -fno-discard-value-names -c -o orderingTwo.bc #unoptimised IR bitcode to run
clang -g unorderedTwo.c -o unorderedTwo

echo "Runtime of unoptimised code (unorderedTwo):"
echo ""

/usr/bin/time -v lli orderingTwo.ll >> output.txt 2>&1
value_a_unordered=$(sed -n "2p" "output.txt") 
echo $value_a_unordered
value_b_unordered=$(sed -n "4p" "output.txt") 
echo $value_b_unordered
value_c_unordered=$(sed -n "6p" "output.txt") 
echo $value_c_unordered
value_d_unordered=$(sed -n "8p" "output.txt") 
echo $value_d_unordered
value_e_unordered=$(sed -n "10p" "output.txt") 
echo $value_e_unordered
echo ""
file_content=$(sed -n "16p" "output.txt") 
echo $file_content
echo ""
echo "Memory usage of unordered AoS:"
file_content=$(sed -n "21p" "output.txt")
echo $file_content
rm output.txt
echo ""

echo "Applying struct field reordering "
echo ""

opt -load-pass-plugin=../../../passes/allPasses.so -passes="reorderAoS" -time-passes < orderingTwo.ll > orderedTwo.bc 2> output.txt
file_content=$(sed -n "1p" "output.txt") 
echo $file_content
echo ""
file_content=$(sed -n "5p" "output.txt") #originally "4p" without any errs() in optimisation pass
echo $file_content
rm output.txt

echo ""

echo "Compiling optimised LLVM code to an executable"

llvm-dis orderedTwo.bc -o orderedTwo.ll > /dev/null 2>&1

echo ""
echo "Runtime of optimised code (ordered AoS):"
echo ""

/usr/bin/time -v lli orderedTwo.ll >> output.txt 2>&1
value_a_ordered=$(sed -n "2p" "output.txt") 
echo $value_a_ordered
value_b_ordered=$(sed -n "4p" "output.txt") 
echo $value_b_ordered
value_c_ordered=$(sed -n "6p" "output.txt") 
echo $value_c_ordered
value_d_ordered=$(sed -n "8p" "output.txt") 
echo $value_d_ordered
value_e_ordered=$(sed -n "10p" "output.txt") 
echo $value_e_ordered
echo ""
file_content=$(sed -n "16p" "output.txt") 
echo $file_content
echo ""
echo "Memory usage of ordered AoS:"
file_content=$(sed -n "21p" "output.txt")
echo $file_content
rm output.txt

echo ""

if [ "$value_a_unordered" == "$value_a_ordered" ] && [ "$value_b_unordered" == "$value_b_ordered" ] && [ "$value_c_unordered" == "$value_c_ordered" ] && [ "$value_d_unordered" == "$value_d_ordered" ] && [ "$value_e_unordered" == "$value_e_ordered" ] 
then
  echo "Outputs after optimisation are correct"
else
  echo "Outputs after optimisation are NOT correct"
fi


echo "-----------------------------------------------------------"
echo "End of all tests"
echo ""



