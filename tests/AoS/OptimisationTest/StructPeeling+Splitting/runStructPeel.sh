#!/bin/bash 

clang -g unpeeledAoS.c -o unpeeledAoS
clang -g peeledAoS.c -o peeledAoS

echo "Runtime of unpeeled AoS:"
echo ""
/usr/bin/time -v ./unpeeledAoS 2> output.txt
file_content=$(sed -n "5p" "output.txt") 
echo $file_content
echo "Memory usage of unpeeled AoS:"
file_content=$(sed -n "10p" "output.txt")
echo $file_content
$(rm output.txt)

echo ""
# # sleep 2

echo "Runtime of peeled AoS:"
echo ""
/usr/bin/time -v ./peeledAoS 2> output.txt
file_content=$(sed -n "5p" "output.txt") 
echo $file_content
echo "Memory usage of peeled AoS:"
file_content=$(sed -n "10p" "output.txt") 
echo $file_content
$(rm output.txt)
