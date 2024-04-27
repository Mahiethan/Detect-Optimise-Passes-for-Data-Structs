#!/bin/bash 

tail -n +30 $1 >> fileOne.txt
tail -n +30 $2 >> fileTwo.txt

cmp --silent fileOne.txt fileTwo.txt && echo "Outputs are identical. Optimisation is valid." || echo "Outputs are different. Optimisation is NOT valid."
rm fileOne.txt
rm fileTwo.txt