add_llvm_pass_plugin(detectAoS
    # List your source files here.
    detectAoS.cpp

    DEPENDS
    intrinsics_gen

    PLUGIN_TOOL
    opt
)