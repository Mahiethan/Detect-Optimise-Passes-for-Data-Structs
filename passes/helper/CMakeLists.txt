add_llvm_pass_plugin(removeConstantGEP
    # List your source files here.
    removeConstantGEP.cpp

    DEPENDS
    intrinsics_gen

    PLUGIN_TOOL
    opt
)