#!/bin/bash 

echo "-----------------------------------------------------------"
echo "Running test one - optimising unpeeledAoS.c"
echo ""

clang -emit-llvm unpeeledAoS.c -fno-discard-value-names -S -o unpeeledStruct.ll #unoptimised IR
clang -emit-llvm unpeeledAoS.c -fno-discard-value-names -c -o unpeeledStruct.bc #unoptimised IR bitcode to run
clang -g unpeeledAoS.c -o unpeeledAoS

echo "Runtime of unoptimised code (unpeeled AoS):"
echo ""

/usr/bin/time -v ./unpeeledAoS >> output.txt 2>&1
value_a_unpeeled=$(sed -n "2p" "output.txt") 
echo $value_a_unpeeled
value_b_unpeeled=$(sed -n "4p" "output.txt") 
echo $value_b_unpeeled
echo ""
file_content=$(sed -n "10p" "output.txt") 
echo $file_content
echo ""
echo "Memory usage of unpeeled AoS:"
file_content=$(sed -n "15p" "output.txt")
echo $file_content
rm output.txt
echo ""

echo "Applying struct peeling"
echo ""

opt -load-pass-plugin=../../../passes/allPasses.so -passes="peelAoS" -time-passes < unpeeledStruct.ll > peeledStruct.bc 2> output.txt
file_content=$(sed -n "4p" "output.txt") #originally "4p" without any errs() in optimisation pass
echo $file_content
rm output.txt

echo ""
echo "Compiling optimised LLVM code to an executable"

llvm-dis peeledStruct.bc -o peeledStruct.ll > /dev/null 2>&1
llc peeledStruct.bc -o peeledStruct.s > /dev/null 2>&1 #compiling to native assembly using the LLC code generator
gcc -no-pie peeledStruct.s -o peeledStruct.native > /dev/null 2>&1 #Assemble the native assembly language file into a program

rm peeledStruct.s

echo ""
echo "Runtime of optimised code (peeled AoS):"
echo ""

/usr/bin/time -v ./peeledStruct.native >> output.txt 2>&1
value_a_peeled=$(sed -n "2p" "output.txt") 
echo $value_a_peeled
value_b_peeled=$(sed -n "4p" "output.txt") 
echo $value_b_peeled
echo ""
file_content=$(sed -n "10p" "output.txt") 
echo $file_content
echo ""
echo "Memory usage of peeled AoS:"
file_content=$(sed -n "15p" "output.txt")
echo $file_content
rm output.txt

echo ""

if [ "$value_a_unpeeled" == "$value_a_peeled" ] && [ "$value_b_unpeeled" == "$value_b_peeled" ] 
then
  echo "Outputs after optimisation are correct"
else
  echo "Outputs after optimisation are NOT correct"
fi

echo "-----------------------------------------------------------"
echo "Running test two - optimising peeledAoS.c"
echo ""

clang -emit-llvm peeledAoS.c -fno-discard-value-names -S -o unpeeledStructTwo.ll #unoptimised IR
clang -emit-llvm peeledAoS.c -fno-discard-value-names -c -o unpeeledStructTwo.bc #unoptimised IR bitcode to run
clang -g peeledAoS.c -o peeledAoS

echo "Runtime of unoptimised code (unpeeled AoS):"
echo ""

/usr/bin/time -v ./peeledAoS >> output.txt 2>&1
value_a_unpeeled=$(sed -n "2p" "output.txt") 
echo $value_a_unpeeled
value_b_unpeeled=$(sed -n "4p" "output.txt") 
echo $value_b_unpeeled
echo ""
file_content=$(sed -n "10p" "output.txt") 
echo $file_content
echo ""
echo "Memory usage of peeled AoS:"
file_content=$(sed -n "15p" "output.txt")
echo $file_content
rm output.txt
echo ""

echo "Applying struct peeling"
echo ""

opt -load-pass-plugin=../../../passes/allPasses.so -passes="peelAoS" -time-passes < unpeeledStructTwo.ll > peeledStructTwo.bc 2> output.txt
file_content=$(sed -n "4p" "output.txt") #originally "4p" without any errs() in optimisation pass
echo $file_content
rm output.txt

echo ""
echo "Compiling optimised LLVM code to an executable"

llvm-dis peeledStructTwo.bc -o peeledStructTwo.ll > /dev/null 2>&1
llc peeledStructTwo.bc -o peeledStructTwo.s > /dev/null 2>&1 #compiling to native assembly using the LLC code generator
gcc -no-pie peeledStructTwo.s -o peeledStructTwo.native > /dev/null 2>&1 #Assemble the native assembly language file into a program

rm peeledStructTwo.s

echo ""
echo "Runtime of optimised code (peeled peeled AoS):"
echo ""

/usr/bin/time -v ./peeledStructTwo.native >> output.txt 2>&1
value_a_peeled=$(sed -n "2p" "output.txt") 
echo $value_a_peeled
value_b_ordered=$(sed -n "4p" "output.txt") 
echo $value_b_peeled
echo ""
file_content=$(sed -n "10p" "output.txt") 
echo $file_content
echo ""
echo "Memory usage of peeled peeled AoS:"
file_content=$(sed -n "15p" "output.txt")
echo $file_content
rm output.txt

if [ "$value_a_unpeeled" == "$value_a_peeled" ] && [ "$value_b_unpeeled" == "$value_b_peeled" ] 
then
  echo "Outputs after optimisation are correct"
else
  echo "Outputs after optimisation are NOT correct"
fi

echo "-----------------------------------------------------------"
echo "End of all tests"
echo ""



