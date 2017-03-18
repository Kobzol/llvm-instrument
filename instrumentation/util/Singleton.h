#pragma once

#include <memory>

template <typename T>
class Singleton
{
public:
    static T& get()
    {
        static T instance;
        return instance;
    }
};