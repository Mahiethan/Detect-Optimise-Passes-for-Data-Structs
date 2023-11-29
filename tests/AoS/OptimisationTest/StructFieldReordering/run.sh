#!/bin/bash 

clang -g unorderedAoS.c -o unorderedAoS
clang -g orderedAoS.c -o orderedAoS

echo "Runtime of unordered AoS (in seconds):"
time ./unorderedAoS
echo "Memory usage of unordered AoS (in KB):"
/usr/bin/time -f %M ./unorderedAoS

echo ""
# sleep 2

echo "Runtime of ordered AoS (in seconds):"
time ./orderedAoS |& grep real
echo "Memory usage of ordered AoS (in KB):"
/usr/bin/time -f %M ./orderedAoS

