#include <fstream>
#include <unordered_set>
#include <iostream>
#include <string>
#include <sstream>

#include "DummySysCall.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Function.h"

using namespace llvm;

bool InjectDummySysCall::runOnModule(Module &M) {
  bool InsertedAtLeastOneSysCall = false;

  std::unordered_map<std::string, int> funcMap;

  std::ifstream file("/home/amit-talawar/Final_Passes/Python Scripts/libc_functions.txt");
  if (!file.is_open()) {
      std::cerr << "Failed to open the text file: " << "\n";
  }

  std::string key;
  int value;
  while(file >> key >> value) {
    funcMap[key] = value;
  }

  file.close();

  for(auto &Func : M) {
    if(!Func.isDeclaration()) {
      for(auto &BB : Func) {
        for(auto &Ins : BB) {
          auto *CB = dyn_cast<CallBase>(&Ins);
          if(nullptr == CB){
            continue;
          }
          
          auto DirectInvoc = CB->getCalledFunction();
          if(nullptr == DirectInvoc) {
            continue;
          }
          
          std::string CFName = DirectInvoc->getName().str().c_str();
          if(funcMap.find(CFName) == funcMap.end()) {
            continue;
          }

          InsertedAtLeastOneSysCall = true;
          LLVMContext &context = Ins.getModule()->getContext();
          IRBuilder<> builder(&Ins);

          // Function *dummySysCall = Ins.getModule()->getFunction("dummy_syscall");
          // if(!dummySysCall) {
          //   FunctionType *syscallType = FunctionType::get(Type::getVoidTy(context), {Type::getInt32Ty(context)}, false);
          //   dummySysCall = Function::Create(syscallType, Function::ExternalLinkage, "dummy_syscall", Ins.getModule());
          // }

          // Value *argVal = ConstantInt::get(Type::getInt32Ty(context), funcMap[CFName]);

          // builder.CreateCall(dummySysCall, {argVal});


          int sys_call_number = 100;  // To be replaced with the registered syscall number, 100 is just a dummy number and it can well refer to another system call, dont run the binaries as it will throw errors. 

          Value *argVal = ConstantInt::get(Type::getInt64Ty(context), funcMap[CFName]);

          // Inline assembly string according to x86 standard
          std::string asmStr = 
              "movq $0, %rax; "  // Load the syscall number into rax (operand 0)
              "movq $1, %rdi; "  // Load the first argument into rdi (operand 1)
              "syscall";  // Invoke the syscall

          InlineAsm *syscallAsm = InlineAsm::get(
              FunctionType::get(Type::getVoidTy(context), {Type::getInt64Ty(context), Type::getInt64Ty(context)}, false),  // 64-bit syscall number, 64-bit argument
              asmStr, "r,r", true);

          builder.CreateCall(syscallAsm, {ConstantInt::get(Type::getInt64Ty(context), sys_call_number), argVal});
        }
      }
    } 
  }

  return InsertedAtLeastOneSysCall;
}

PreservedAnalyses InjectDummySysCall::run(llvm::Module &M,
                                       llvm::ModuleAnalysisManager &) {
  bool Changed =  runOnModule(M);

  return (Changed ? llvm::PreservedAnalyses::none()
                  : llvm::PreservedAnalyses::all());
}


llvm::PassPluginLibraryInfo getInjectDummySysCallPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "inject-dummy-sys-call", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, ModulePassManager &MPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "inject-dummy-sys-call") {
                    MPM.addPass(InjectDummySysCall());
                    return true;
                  }
                  return false;
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getInjectDummySysCallPluginInfo();
}
