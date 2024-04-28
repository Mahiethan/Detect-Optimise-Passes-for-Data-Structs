# Programs used for detection of AoS data structures

## To recompile the existing demo programs:

```bash
./recompileDemoPrograms.sh
```

## To produce an .ll file (LLVM IR) for your custom program:

```bash
clang -emit-llvm {yourCustomProgram.c} -fno-discard-value-names -S -o {yourCustomProgram.ll}
```

Replace {yourCustomProgram.c} with your custom program name (including the `.c` extension).

Replace {yourCustomProgram.ll} with your desired output name, which MUST include the `.ll` extension.

## To apply an optimisation to a particular program:

```bash
opt -load-pass-plugin=../../../passes/allPasses.so -passes="detectAoSoA" < {INPUT_IR_FILENAME} > {OUTPUT_IR_FILENAME}  
```

Replace `{IR_FILENAME}` with any one of the available `.ll` files.

Replace `{OUTPUT_IR_FILENAME}` with:
-  your desired output file name with extension `.ll` (e.g. `output.ll`)
OR 
- use `\dev\null` if you don't want any output IR.

The value given to the `-passes` flag can be changed to any other available compiler pass.

## *Navigation*:

- [Main README](../../../README.md)
- [./passes README](../../../passes/README.md)
- [./test-results README](../../../test-results/README.md)
- [./demo-programs README](../../README.md)