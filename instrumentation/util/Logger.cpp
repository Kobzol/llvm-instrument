#include "Logger.h"

Logger::Logger()
{

}

void Logger::printf(const char* format) const
{
    std::cerr << format;
}

void Logger::indent(int count) const
{
    if (count <= 0) return;

    std::string spaces(static_cast<unsigned int>(count * 2), ' ');
    this->printf(spaces.c_str());
}

bool Logger::isEnabled() const
{
    return this->enabled;
}
void Logger::enable()
{
    this->enabled = true;
}
void Logger::disable()
{
    this->enabled = false;
}

void Logger::setPriority(int priority)
{
    this->priority = priority;
}

bool Logger::checkPriority(int priority) const
{
    return priority >= this->priority;
}

void Logger::dump(z3::ast& ast) const
{
    const char* str = Z3_ast_to_string(ast.ctx(), ast);
    this->printf(str);
}
