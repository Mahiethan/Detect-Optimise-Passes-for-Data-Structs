#/bin/bash
find . -type d -name "CMakeFiles"  -execdir rm -r "CMakeFiles" ';'
find . -name "cmake_install.cmake" -delete
find . -name "CMakeCache.txt" -delete
find . -name "Makefile" -delete