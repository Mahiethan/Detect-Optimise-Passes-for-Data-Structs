# How to compile the passes?

## Pre-requisites:

- CMake (version 3.6 or newer) are required, which should be present on the DCS lab machines and DCS Batch Compute systems.

## Steps

1. Run `cmake .` in the current directory at `./passes`

2. Once successful, run `make` to generate the shared object files (`.so`) for each compiler pass, which can be seen in the named folders

3. The file `allPasses.so` will be the main top-level compiler pass that is to be run. Other compiler passes inside the named subdirectories are used by this `allPasses.so` files and can be run independently, but it may not work as expected due to dependencies between the passes.S

4. Run the `clean.sh` bash script below in the current directory `./passes` folder to remove the intermediary files produced by the cmake in all subdirectories.

```bash
source clean.sh
```

## Navigation:

- [Main README](../README.md)
- [./passes README](./README.md)
- [./test-results README](../test-results/README.md)
<!-- - [./demo-programs README](../demo-programs/README.md) -->