add_llvm_library( LLVMOurPass MODULE
  OurPass.cpp

  PLUGIN_TOOL
  opt
  )
