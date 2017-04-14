#include "PathCondition.h"
#include "../util/Logger.h"

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
    Logger::log("PC: %s\n", Logger::stringify(this->condition));
}

bool PathCondition::isSatisfiable(const expr& expr)
{
    solver solver(*this->ctx);
    solver.add(this->condition && expr);

    return solver.check() == check_result::sat;
}
