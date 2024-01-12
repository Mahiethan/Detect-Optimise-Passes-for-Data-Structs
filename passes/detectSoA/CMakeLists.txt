add_llvm_pass_plugin(detectSoA
    # List your source files here.
    detectSoA.cpp


    PLUGIN_TOOL
    opt
)