add_llvm_pass_plugin(reorderAoS
    # List your source files here.
    reorderAoS.cpp

    DEPENDS
    intrinsics_gen

    PLUGIN_TOOL
    opt
)