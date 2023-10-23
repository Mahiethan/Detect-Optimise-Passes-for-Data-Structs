# Using clang to produce .ll files (assembly code)

```bash
clang -emit-llvm static_AoS.c -S -o staticAoS.ll
```
Flags `-emit-llvm` and `-S` will produce a .ll file.
Flag `-o` specifies the output file name.

# Using clang to produce .ll files (bitcode)

```bash
clang -emit-llvm static_AoS.c -c -o staticAoS.ll
```
Flags `-emit-llvm -c` will produce a .bc file

# Running a compiler pass using `opt`

```bash
opt -load-pass-plugin=../../location/to/pass -passes="passName"  < fileToTest.bc > /dev/null
```
fileToTest can also be a .ll file.

`/dev/null` can be replaced with `fileName.bc` if you want to generate the optimisied/output bitcode file.

Use flag `--time-passes` to print out the pass execution timing report.

```bash
opt -load-pass-plugin=../../location/to/pass -passes="passName" -time-passes < fileToTest.bc > /dev/null
```
# Running unit tests

To run a `.bat` file, do:

```bash
bats fileName.bats
```
A bash script to run all `.bats` files (all unit tests) will be created soon...

