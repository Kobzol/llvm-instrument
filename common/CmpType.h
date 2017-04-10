#pragma once

#include <cstddef>

enum class CmpType : size_t
{
    Equal = 1,
    NotEqual,
    SGE,
    SGT,
    SLE,
    SLT
};
