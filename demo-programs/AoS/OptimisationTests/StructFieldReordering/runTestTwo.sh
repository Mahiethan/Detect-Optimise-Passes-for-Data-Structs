#!/bin/bash 

clang -g unorderedTwo.c -o unorderedTwo
clang -g orderedTwo.c -o orderedTwo

echo "Runtime of unordered AoS:"
echo ""
/usr/bin/time -v ./unorderedTwo >> output.txt 2>&1
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
echo "Memory usage of unordered AoS:"
file_content=$(sed -n "21p" "output.txt")
echo $file_content
rm output.txt

echo ""
# # sleep 2

echo "Runtime of ordered AoS:"
echo ""
/usr/bin/time -v ./orderedTwo >> output.txt 2>&1
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
echo ""