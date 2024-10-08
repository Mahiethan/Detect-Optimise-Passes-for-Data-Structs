cmake_minimum_required(VERSION 3.6)

# LLVM uses C++17.
set(CMAKE_CXX_STANDARD 17)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Load LLVMConfig.cmake. If this fails, consider setting `LLVM_DIR` to point
# to your LLVM installation's `lib/cmake/llvm` directory.
find_package(LLVM REQUIRED CONFIG)

# Include the part of LLVM's CMake libraries that defines
# `add_llvm_pass_plugin`.
include(AddLLVM)

# Use LLVM's preprocessor definitions, include directories, and library search
# paths.
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

# Add new pass directories here:
# add_subdirectory(./helper)

# add_subdirectory(./detectAoS)
# add_subdirectory(./detectSoA)
# add_subdirectory(./detectAoS/detectAoSFlags)
# add_subdirectory(./optimiseAoS/reorderAoS)
# add_subdirectory(./optimiseAoS/peelAoS)
# add_subdirectory(./optimiseAoS/splitAoS)

add_llvm_pass_plugin(allPasses
    # List your source files here.
    allPasses.cpp

    DEPENDS
    intrinsics_gen

    PLUGIN_TOOL
    opt
)