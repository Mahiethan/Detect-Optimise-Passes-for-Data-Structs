Currently, splitAoS optimisation pass works with small-sized AoS variables. 

Run the following command to optimise the small AoS present in unsplitAoS.c

```bash
opt -load-pass-plugin=../../../../passes/allPasses.so -passes="splitAoS" < unsplitAoS.ll > test.bc && llvm-dis test.bc
```

Run both `unsplitAoS.ll` and `test.ll` and compare the results and time - results should be same and `test.ll` should be quicker

```bash
/usr/bin/time -v lli unsplitAoS.ll && /usr/bin/time -v lli test.ll
```
Run valgrind on both files with the following command.
```bash
valgrind ./unsplitAoS && valgrind ./test.ll
```
There are no memory leaks in `unsplitAoS.ll` **but** memory leaks exist for `test.ll`

Therefore, need to free the structs allocated to the AoS elements - otherwise too much memory is leaked and it will crash

**Also**, only create `malloc()` for a cold pointer struct if it hasn't been allocated already. Can check this by using a map data structure to store the AoS to optimise, and the array indices that have its cold pointer struct malloc-ed. 

MOVE THESE NOTES TO TRELLO OR THE STRUCT_SPLITTING NOTES ONCE THIS HAS BEEN IMPLEMENTED WITHOUT FAULTS