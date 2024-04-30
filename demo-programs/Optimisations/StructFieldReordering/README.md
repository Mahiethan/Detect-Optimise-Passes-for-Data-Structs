# Programs used for Structure Field Reordering

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
opt -load-pass-plugin=../../../passes/allPasses.so -passes="reorderAoS" < {INPUT_IR_FILENAME} > {OUTPUT_IR_FILENAME}  
```

Replace `{IR_FILENAME}` with any one of the available `.ll` files.

Replace `{OUTPUT_IR_FILENAME}` with:
-  your desired output file name with extension `.ll` (e.g. `output.ll`)
OR 
- use `/dev/null` if you don't want any output IR.

The value given to the `-passes` flag can be changed to any other available compiler pass.

## To run test script for structure field reordering optimisation using compiler pass:

```bash
source runOpt.sh
```

This will apply the `reorderAoS` compiler on two separate C programs: `unorderedAoS.c` and `unorderedTwo.c`

This shows the comparison between the execution times and memory usages of both sets of unoptimised and optimised programs, as well as checking whether the outputs match.

## To run the first set of unordered and ordered programs (with optimisations manually applied):

```bash
source runProgramsOne.sh
```

This shows the comparison between the execution times and memory usages of both unoptimised and optimised programs, as well as checking whether the outputs match.

## To run the second set of unordered and ordered programs (with optimisations manually applied):

```bash
source runProgramsTwo.sh
```

This shows the comparison between the execution times and memory usages of both unoptimised and optimised programs, as well as checking whether the outputs match.

## *Navigation*:

- [Main README](../../../README.md)
- [./passes README](../../../passes/README.md)
- [./test-results README](../../../test-results/README.md)
- [./demo-programs README](../../README.md)