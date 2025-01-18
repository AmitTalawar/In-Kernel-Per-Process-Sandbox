#ifndef DUMMYSYSCALL_H
#define DUMMYSYSCALL_H

#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"

struct InjectDummySysCall : public llvm::PassInfoMixin<InjectDummySysCall> {
  llvm::PreservedAnalyses run(llvm::Module &M,
                              llvm::ModuleAnalysisManager &);
  bool runOnModule(llvm::Module &M);

  static bool isRequired() { return true; }
};

#endif