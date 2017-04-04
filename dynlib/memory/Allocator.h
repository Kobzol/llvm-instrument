#pragma once

class Allocator
{
protected:
    char* align(char* address)
    {
        size_t aligned = reinterpret_cast<size_t>(address);
        aligned &= ~ (size_t) 0x07;

        if (aligned == reinterpret_cast<size_t>(address))
        {
            return address;
        }
        else
        {
            return reinterpret_cast<char*>(aligned + 0x08);
        }
    }
};
