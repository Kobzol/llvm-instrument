#include "Call.h"

#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <util/Util.h>
#include <llvm/IR/Constants.h>


void Call::handle(llvm::Module* module, llvm::Instruction* instruction)
{
    /*llvm::CallInst* call = static_cast<llvm::CallInst*>(instruction);
    std::string functionName = call->getCalledFunction()->getName().str();

    llvm::LLVMContext& ctx = module->getContext();
    std::string demangled = demangleFunction(functionName);
    llvm::Type* int64 = llvm::IntegerType::get(ctx, 64);
    llvm::PointerType* int64ptr = int64->getPointerTo();
    llvm::Type* voidType = llvm::Type::getVoidTy(ctx);

    if (demangled == "operator new(unsigned long)")
    {
        llvm::Constant* callback;
        callback = module->getOrInsertFunction("__se_store_const", voidType, int64ptr, int64, nullptr);

        llvm::Function* fn = llvm::cast<llvm::Function>(callback);

        llvm::Instruction* cast = llvm::CastInst::CreateSExtOrBitCast(call, int64ptr, "", call->getNextNode());
        llvm::CallInst::Create(fn, { cast, call->getArgOperand(0) }, "", cast->getNextNode());
    }*/
}
