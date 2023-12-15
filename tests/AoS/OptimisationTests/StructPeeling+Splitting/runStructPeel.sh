#!/bin/bash 

clang -g unpeeledAoS.c -o unpeeledAoS
clang -g peeledAoS.c -o peeledAoS

echo "Runtime of unpeeled AoS:"
echo ""
/usr/bin/time -v ./unpeeledAoS >> output.txt 2>&1
value_a_unpeeled=$(sed -n "2p" "output.txt") 
echo $value_a_unpeeled
value_b_unpeeled=$(sed -n "4p" "output.txt") 
echo $value_b_unpeeled
file_content=$(sed -n "10p" "output.txt") 
echo $file_content
echo "Memory usage of unpeeled AoS:"
file_content=$(sed -n "15p" "output.txt")
echo $file_content
rm output.txt

echo ""
# # sleep 2

echo "Runtime of peeled AoS:"
echo ""
/usr/bin/time -v ./peeledAoS >> output.txt 2>&1
value_a_peeled=$(sed -n "2p" "output.txt") 
echo $value_a_peeled
value_b_peeled=$(sed -n "4p" "output.txt") 
echo $value_b_peeled
file_content=$(sed -n "10p" "output.txt") 
echo $file_content
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
echo ""
