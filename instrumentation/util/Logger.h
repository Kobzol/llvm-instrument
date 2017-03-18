#pragma once

#include <iostream>
#include <z3++.h>

#include "Singleton.h"

class Logger : public Singleton<Logger>
{
    friend class Singleton<Logger>;

public:
    enum Prio: unsigned int
    {
        DEBUG = 0,
        INFO = 1,
        WARNING = 2,
        ERROR = 3
    };

    template <typename... Targs>
    bool line(int priority, int indent, std::string format, Targs... args) const
    {
        if (this->log(priority, indent, format, args...))
        {
            std::cerr << std::endl;
            return true;
        }

        return false;
    }
    template <typename... Targs>
    bool line(int priority, std::string format, Targs... args) const
    {
        return this->line(priority, 0, format, args...);
    }
    template <typename... Targs>
    bool line(std::string format, Targs... args) const
    {
        return this->line(Logger::DEBUG, 0, format, args...);
    }

    template <typename... Targs>
    bool log(int priority, int indent, std::string format, Targs... args) const
    {
        if (this->isEnabled() && this->checkPriority(priority))
        {
            this->indent(indent);
            this->printf(format.c_str(), args...);
            return true;
        }

        return false;
    }
    template <typename... Targs>
    bool log(int priority, std::string format, Targs... args) const
    {
        return this->log(priority, 0, format, args...);
    }
    template <typename... Targs>
    bool log(std::string format, Targs... args) const
    {
        return this->log(Logger::DEBUG, 0, format, args...);
    }

    void indent(int count) const;

    void setPriority(int priority);
    bool checkPriority(int priority) const;

    bool isEnabled() const;
    void enable();
    void disable();

    void dump(z3::ast& ast) const;

protected:
    Logger();

private:
    void printf(const char* format) const;

    template<typename T, typename... Targs>
    void printf(const char* format, T value, Targs... args) const
    {
        for (; *format != '\0'; format++)
        {
            if (*format == '%')
            {
                std::cerr << value;
                printf(format + 1, args...);
                return;
            }
            std::cerr << *format;
        }
    }

    int priority = 0;
    bool enabled = true;
};
