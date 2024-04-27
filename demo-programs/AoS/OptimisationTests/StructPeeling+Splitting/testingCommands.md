# To optimise unsplitAoS.c

```bash
opt -load-pass-plugin=../../../../passes/allPasses.so -passes="splitAoS" < unsplitAoS.ll > testUnsplitAoS.bc && llvm-dis testUnsplitAoS.bc

clang -g testUnsplitAoS.ll -o testUnsplitAoS

/usr/bin/time -v lli unsplitAoS.ll && /usr/bin/time -v lli testUnsplitAoS.ll
```

# To optimise unsplitTwo.c

```bash
opt -load-pass-plugin=../../../../passes/allPasses.so -passes="splitAoS" < unsplitTwo.ll > testUnsplitTwo.bc && llvm-dis testUnsplitTwo.bc

clang -g testUnsplitTwo.ll -o testUnsplitTwo

/usr/bin/time -v lli unsplitTwo.ll && /usr/bin/time -v lli testUnsplitTwo.ll
```