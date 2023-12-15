#!/bin/bash 

clang -g unsplitAoS.c -o unsplitAoS
clang -g splitAoS.c -o splitAoS

echo "Runtime of unsplit AoS:"
echo ""
/usr/bin/time -v ./unsplitAoS 2> output.txt
file_content=$(sed -n "5p" "output.txt") 
echo $file_content
echo "Memory usage of unsplit AoS:"
file_content=$(sed -n "10p" "output.txt")
echo $file_content
# $(rm output.txt)

echo ""
# # sleep 2

echo "Runtime of split AoS:"
echo ""
/usr/bin/time -v ./splitAoS 2> output.txt
file_content=$(sed -n "5p" "output.txt") 
echo $file_content
echo "Memory usage of split AoS:"
file_content=$(sed -n "10p" "output.txt") 
echo $file_content
# $(rm output.txt)
