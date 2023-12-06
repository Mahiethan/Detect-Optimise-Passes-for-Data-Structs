#!/bin/bash 

clang -g unorderedAoS.c -o unorderedAoS
clang -g orderedAoS.c -o orderedAoS

echo "Runtime of unordered AoS:"
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
echo "Memory usage of unordered AoS:"
file_content=$(sed -n "19p" "output.txt")
echo $file_content
rm output.txt

echo ""
# # sleep 2

echo "Runtime of ordered AoS:"
echo ""
/usr/bin/time -v ./orderedAoS >> output.txt 2>&1
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
echo "Memory usage of ordered AoS:"
file_content=$(sed -n "19p" "output.txt") 
echo $file_content
rm output.txt

echo ""
if [ "$value_a_unordered" == "$value_a_ordered" ] && [ "$value_b_unordered" == "$value_b_ordered" ] && [ "$value_c_unordered" == "$value_c_ordered" ] && [ "$value_d_unordered" == "$value_d_ordered" ] 
then
  echo "Outputs after optimisation are correct"
fi
echo ""