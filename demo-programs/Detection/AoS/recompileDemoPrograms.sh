#/bin/bash

## dynamic_AoS.c

fileName="dynamic_AoS.c"
outputFileName="dynamicAoS"

clang -g "$fileName" -o "$outputFileName"

clang -emit-llvm -fno-discard-value-names "$fileName" -S -o "$outputFileName".ll

## noAoS.c

fileName="noAoS.c"
outputFileName="noAoS"

clang -g "$fileName" -o "$outputFileName"

clang -emit-llvm -fno-discard-value-names "$fileName" -S -o "$outputFileName".ll

## static_AoS.c

fileName="static_AoS.c"
outputFileName="staticAoS"

clang -g "$fileName" -o "$outputFileName"

clang -emit-llvm -fno-discard-value-names "$fileName" -S -o "$outputFileName".ll

