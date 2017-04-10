#include "PathCondition.h"
#include "../util/Logger.h"

#include <z3.h>

using namespace z3;

PathCondition::PathCondition(context* ctx): ctx(ctx), condition(ctx->bool_val(true))
{

}

void PathCondition::addCondition(expr expr)
{
    this->condition = this->condition && expr;
}

void PathCondition::dump()
{
    const char* str = Z3_ast_to_string(*this->ctx, this->condition);
    Logger::log("PC: %s\n", str);
}
