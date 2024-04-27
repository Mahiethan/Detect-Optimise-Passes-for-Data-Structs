# Test results used in final report:

This directory contains several subdirectories that contains test results for each optimisation pass (reorderAos, peelAoS and splitAoS), which were discussed in the final report.

Within these subdirectories, there are two folders that contains the results for multiple AoS sizes used for running the unoptimised and optimised programs. 

The `.c` source code files, the IR representations (`.ll` files) and executable binaries all represent the test programs used for each optimisation. Feel free to view the code for these programs and run the executables. 

The bash scripts (`.sh` files) were used to run and compile the tests using the compiler passes, as well as comparing the outputs between the unoptimised and optimised programs. These files remain within this project to show the automated testing process that was used, and can be used to compile the programs so the executables can be run on your current machine. **If you want to do this, ensure that your newly-compiled `allPasses.so` file in `../passes` is copied into the `./DCSBatch` folder, replacing the old file.**

Note that the following executables **require a command line argument** to be provided in order to specify the **size of the AoS**, otherwise a segmentation fault will appear in the terminal:

- `./reorderAoS/unordered`
- `./reorderAoS/ordered`
- `./reorderAoS/optimised`
- `./splitAoS/unsplit`
- `./splitAoS/optimised`

The executables inside the directory `./peelAoS` do not require a command line argument as the AoS size is defined inside the source code of `unpeeled.c`. The default value is `1000` and this value can be changed from the `SIZE` variable.

## Navigation:

- [Main README](../README.md)
- [./passes README](../passes/README.md)
- [./test-results README](./README.md)
<!-- - [./demo-programs README](../demo-programs/README.md) -->