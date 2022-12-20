#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace {
struct OurPass: public FunctionPass {
  static char ID;
  OurPass() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    errs() << "OurPass: ";
    errs().write_escaped(F.getName()) << '\n';
    return false;
  }
}; // end of struct OurPass
}  // end of anonymous namespace

char OurPass::ID = 0;
static RegisterPass<OurPass> X("OurPass", "Hello World Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
