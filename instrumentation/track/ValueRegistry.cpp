#include "ValueRegistry.h"

using namespace llvm;

size_t ValueRegistry::getId(Value* value)
{
    if (!this->hasId(value))
    {
        this->insertId(value);
    }

    return this->map[this->hashId(value)];
}

void ValueRegistry::insertId(Value* value)
{
    this->map.insert({this->hashId(value), this->map.size()});
}

bool ValueRegistry::hasId(llvm::Value* value) const
{
    return this->map.count(this->hashId(value)) > 0;
}

size_t ValueRegistry::hashId(llvm::Value* value) const
{
    return reinterpret_cast<size_t>(value);
}
