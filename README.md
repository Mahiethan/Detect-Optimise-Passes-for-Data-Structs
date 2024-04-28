# Detection and Optimisation of Data Structures in Compile Time - Third Year Project - Source code

<u>Author</u>: Mahiethan Nitharsan \
<u>University ID</u>: 2158063

## Pre-requisites:

- Please run the compiler passes on the DCS lab machines, since it has a working version of LLVM required for this project. Otherwise, please install the version LLVM-18 or newer.

- Alternatively, to test the compiler passes on multiple files simultaneously, or to get more accurate results, you can run this project on the DCS Batch Compute system. This platform works with this project, as it was used to collect the test results.

## How to run the code:

### 1)  Compiling the passes

The compiler passes are found in `./passes`. The existing `.so` files were already compiled on the lab machines, and there might be a chance that it will not work on the current computer this project is running on (especially if you are not running this on the DCS lab machines). Therefore, please re-compile the passes using the included `README.md` file in that folder which describes the steps for compiling the passes into `.so` files.

### 2)  Running the compiler passes

After compiling the passes, feel free to utilise them with the C programs stored in the folder `./demo-programs`. This folder contains specific C programs that were used to test the functionality of the compiler passes during the implementation, and these compiler passes are guaranteed to work on these test programs.

Additionally, you can insert your own custom C programs and test the compiler passes on them. A [`README`](./demo-programs/README.md) file is included in this folder to which leads to other README files that give instructions on how to apply the compiler passes on C programs.

## Test results:

Please see the [README](./test-results/README.md) file in `./test-results` folder for more information about this directory.

## Written project deliverables:

The Specification, Progress Report and the Final Report are all available within this project as `.pdf` files, which can be viewed alongside the source code and test results of this project.

## *Navigation*:

- [Main README](./README.md)
- [./passes README](./passes/README.md)
- [./test-results README](./test-results/README.md)
- [./demo-programs README](./demo-programs/README.md)
