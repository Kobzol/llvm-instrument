#include "Store.h"

#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>

#include "util/DebugUtil.h"

using namespace llvm;

void Store::handle(Module* module, Instruction* instruction)
{
    StoreInst* st = static_cast<StoreInst*>(instruction);
    LLVMContext& ctx = module->getContext();
    Type* int64 = IntegerType::get(ctx, 64);
    PointerType* int64ptr = int64->getPointerTo();
    Type* voidType = Type::getVoidTy(ctx);

    Constant* callback;
    Value* dst = st->getPointerOperand();
    Value* dstSize = ConstantInt::get(int64, dst->getType()->getContainedType(0)->getPrimitiveSizeInBits(), false);
    Value* src = st->getValueOperand();
    Value* srcSize = ConstantInt::get(int64, src->getType()->getPrimitiveSizeInBits(), false);

    if (isa<Constant>(src))
    {
        src = CastInst::CreateSExtOrBitCast(src, int64, "", st);
        callback = module->getOrInsertFunction("__se_store_const", voidType, int64ptr, int64, int64, int64, nullptr);
    }
    else
    {
        while (isa<LoadInst>(src))
        {
            src = cast<LoadInst>(src)->getPointerOperand();
        }

        srcSize = ConstantInt::get(int64, src->getType()->getContainedType(0)->getPrimitiveSizeInBits(), false);
        src = CastInst::CreateSExtOrBitCast(src, int64ptr, "", st);
        callback = module->getOrInsertFunction("__se_store_mem", voidType, int64ptr, int64, int64ptr, int64, nullptr);
    }

    dst = CastInst::CreateSExtOrBitCast(dst, int64ptr, "", st);

    Function* fn = cast<Function>(callback);
    CallInst::Create(fn, { dst, dstSize, src, srcSize }, "", st);
}
