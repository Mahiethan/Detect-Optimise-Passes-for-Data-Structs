#!/bin/bash 

clang -g unorderedAoS.c -o unorderedAoS
clang -g orderedAoS.c -o orderedAoS

echo "Runtime of unordered AoS:"
echo ""
$(/usr/bin/time -v ./unorderedAoS 2> output.txt)
file_content=$(sed -n "5p" "output.txt") 
echo $file_content
echo "Memory usage of unordered AoS:"
file_content=$(sed -n "10p" "output.txt")
echo $file_content
$(rm output.txt)

echo ""
# # sleep 2

echo "Runtime of ordered AoS:"
echo ""
$(/usr/bin/time -v ./orderedAoS 2> output.txt)
file_content=$(sed -n "5p" "output.txt") 
echo $file_content
echo "Memory usage of ordered AoS:"
file_content=$(sed -n "10p" "output.txt") 
echo $file_content
$(rm output.txt)