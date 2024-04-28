#/bin/bash

## disableStructSplit.c

fileName="disableStructSplit.c"
outputFileName="disableStructSplit"

clang -g "$fileName" -o "$outputFileName"

clang -emit-llvm -fno-discard-value-names "$fileName" -S -o "$outputFileName".ll

## splitAoS.c

fileName="splitAoS.c"
outputFileName="splitAoS"

clang -g "$fileName" -o "$outputFileName"

clang -emit-llvm -fno-discard-value-names "$fileName" -S -o "$outputFileName".ll

## splitTwo.c

fileName="splitTwo.c"
outputFileName="splitTwo"

clang -g "$fileName" -o "$outputFileName"

clang -emit-llvm -fno-discard-value-names "$fileName" -S -o "$outputFileName".ll

## unsplitAoS.c

fileName="unsplitAoS.c"
outputFileName="unsplitAoS"

clang -g "$fileName" -o "$outputFileName"

clang -emit-llvm -fno-discard-value-names "$fileName" -S -o "$outputFileName".ll

## unsplitThree.c

fileName="unsplitThree.c"
outputFileName="unsplitThree"

clang -g "$fileName" -o "$outputFileName"

clang -emit-llvm -fno-discard-value-names "$fileName" -S -o "$outputFileName".ll

## unsplitTwo.c

fileName="unsplitTwo.c"
outputFileName="unsplitTwo"

clang -g "$fileName" -o "$outputFileName"

clang -emit-llvm -fno-discard-value-names "$fileName" -S -o "$outputFileName".ll

