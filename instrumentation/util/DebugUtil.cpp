#include "DebugUtil.h"

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/IntrinsicInst.h>
#include <llvm/IR/Module.h>

DebugUtil::DebugUtil()
{

}

DebugInfo* DebugUtil::getDebugInfo(llvm::Value* inst)
{
    return this->getFromCache(inst);
}
std::string DebugUtil::getInstructionLocation(llvm::Instruction* inst)
{
    auto& debugLoc = inst->getDebugLoc();
    llvm::DILocation* loc = debugLoc.get();

    if (loc == nullptr) return "";

    return loc->getFilename().str() + ":" + std::to_string(debugLoc.getLine());
}

const llvm::MDNode* DebugUtil::findVarInFunction(const llvm::Function* function, const llvm::Value* value)
{
    for (auto& block : function->getBasicBlockList())
    {
        for (auto& inst : block.getInstList())
        {
            if (const llvm::DbgDeclareInst* DbgDeclare = llvm::dyn_cast<llvm::DbgDeclareInst>(&inst))
            {
                if (DbgDeclare->getAddress() == value)
                {
                    return DbgDeclare->getVariable();
                }
            }
            else if (const llvm::DbgValueInst* DbgValue = llvm::dyn_cast<llvm::DbgValueInst>(&inst))
            {
                if (DbgValue->getValue() == value)
                {
                    return DbgValue->getVariable();
                }
            }
        }
    }

    return nullptr;
}

const llvm::Function* DebugUtil::findVarScope(const llvm::Value* value)
{
    if (const llvm::Argument* Arg = llvm::dyn_cast<llvm::Argument>(value))
    {
        return Arg->getParent();
    }
    if (const llvm::Instruction* I = llvm::dyn_cast<llvm::Instruction>(value))
    {
        return I->getParent()->getParent();
    }

    return nullptr;
}

std::unique_ptr<DebugInfo> DebugUtil::getGlobalVarDebugInfo(const llvm::GlobalVariable* global)
{
    const llvm::Module* module = global->getParent();
    if (llvm::NamedMDNode* cu = module->getNamedMetadata("llvm.dbg.cu"))
    {
        for (unsigned i = 0, e = cu->getNumOperands(); i != e; ++i)
        {
            auto* compileUnit = llvm::cast<llvm::DICompileUnit>(cu->getOperand(i));
            /*for (llvm::DIGlobalVariableExpression* globalDI : compileUnit->getGlobalVariables())
            {
                if (globalDI->getVariable()-> == global)
                {
                    return std::make_unique<DebugInfo>(globalDI);
                }
            }*/
        }
    }

    return std::make_unique<DebugInfo>();
}

std::unique_ptr<DebugInfo> DebugUtil::getVarDebugInfo(const llvm::Value* value)
{
    if (const llvm::GlobalVariable* global = llvm::dyn_cast<llvm::GlobalVariable>(value))
    {
        return this->getGlobalVarDebugInfo(global);
    }

    const llvm::Function* function = this->findVarScope(value);
    if (!function)
    {
        return std::make_unique<DebugInfo>(value->getName().str());
    }

    const llvm::MDNode* debugVar = this->findVarInFunction(function, value);
    if (!debugVar)
    {
        return std::make_unique<DebugInfo>();
    }

    return std::make_unique<DebugInfo>(static_cast<const llvm::DIVariable*>(debugVar));
}

DebugInfo* DebugUtil::getFromCache(const llvm::Value* value)
{
    if (!this->debugCache.count(value))
    {
        this->debugCache[value] = this->getVarDebugInfo(value);
    }

    return this->debugCache.at(value).get();
}
