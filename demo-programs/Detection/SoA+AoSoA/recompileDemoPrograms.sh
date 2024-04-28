#/bin/bash

## AoSoA.c

fileName="AoSoA.c"
outputFileName="AoSoA"

clang -g "$fileName" -o "$outputFileName"

clang -emit-llvm -fno-discard-value-names "$fileName" -S -o "$outputFileName".ll

## globalPointerSoA.c

fileName="globalPointerSoA.c"
outputFileName="globalPointerSoA"

clang -g "$fileName" -o "$outputFileName"

clang -emit-llvm -fno-discard-value-names "$fileName" -S -o "$outputFileName".ll

## globalSoA.c

fileName="globalSoA.c"
outputFileName="globalSoA"

clang -g "$fileName" -o "$outputFileName"

clang -emit-llvm -fno-discard-value-names "$fileName" -S -o "$outputFileName".ll

## normalSoA.c

fileName="normalSoA.c"
outputFileName="normalSoA"

clang -g "$fileName" -o "$outputFileName"

clang -emit-llvm -fno-discard-value-names "$fileName" -S -o "$outputFileName".ll

## pointerSoA.c

fileName="pointerSoA.c"
outputFileName="pointerSoA"

clang -g "$fileName" -o "$outputFileName"

clang -emit-llvm -fno-discard-value-names "$fileName" -S -o "$outputFileName".ll

