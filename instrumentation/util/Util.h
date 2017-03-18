#pragma once

#include <string>
#include <memory>
#include <cxxabi.h>
#include <cstring>


std::string demangleFunction(std::string name)
{
    std::unique_ptr<char, void(*)(void*)> demangled = std::unique_ptr<char, void(*)(void*)>(
            abi::__cxa_demangle(name.c_str(), NULL, NULL, NULL),
            std::free
    );

    if (!demangled.get()) return "";

    size_t count = std::strlen(demangled.get());
    return std::string(demangled.get(), count);
}
