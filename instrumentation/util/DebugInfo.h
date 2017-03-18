#pragma once

#include <iostream>

namespace llvm {
    class DIVariable;
}

struct DebugInfo
{
public:
    DebugInfo();
    DebugInfo(std::string name, std::string filename = "", int line = -1);
    DebugInfo(const llvm::DIVariable* debugVariable);

    bool hasName() const;
    std::string getName() const;
    std::string getFilename() const;
    int getLine() const;
    bool isValid() const;

    explicit operator bool() const;

    void print(std::ostream& o = std::cerr) const;

private:
    std::string name;
    std::string filename;
    int line;
    bool valid = false;
};

std::ostream& operator<<(std::ostream& o, const DebugInfo& info);
