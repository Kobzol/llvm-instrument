#include "DebugInfo.h"

#include <llvm/IR/DebugInfoMetadata.h>

DebugInfo::DebugInfo()
{

}
DebugInfo::DebugInfo(std::string name, std::string filename, int line)
        : name(name), filename(filename), line(line), valid(true)
{
    if (name == "")
    {
        this->valid = false;
    }
}
DebugInfo::DebugInfo(const llvm::DIVariable* debugVariable)
        : name(debugVariable->getName()), filename(debugVariable->getFilename()), line(debugVariable->getLine()), valid(true)
{

}

bool DebugInfo::hasName() const
{
    return this->getName() != "";
}
std::string DebugInfo::getName() const
{
    return this->name;
}
std::string DebugInfo::getFilename() const
{
    return this->filename;
}
int DebugInfo::getLine() const
{
    return this->line;
}

bool DebugInfo::isValid() const
{
    return false;
}
DebugInfo::operator bool() const
{
    return this->isValid();
}

void DebugInfo::print(std::ostream& o) const
{
    if (this->valid)
    {
        o << this->name << " at " << this->filename << ":" << this->line << std::endl;
    }
    else o << "Invalid location" << std::endl;
}

std::ostream& operator<<(std::ostream& o, const DebugInfo& info)
{
    info.print(o);
    return o;
}
