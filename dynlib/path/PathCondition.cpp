#include "PathCondition.h"
#include "../util/Logger.h"

PathCondition::PathCondition(z3::context* ctx): ctx(ctx), condition(ctx->bool_val(true))
{

}

void PathCondition::addCondition(z3::expr expr)
{
    this->condition = this->condition && expr;
}

void PathCondition::dump()
{
    Logger::log("PC: %s\n", Logger::stringify(this->condition));
}

bool PathCondition::isSatisfiable(const z3::expr& expr, const z3::expr& indexer, z3::expr& model)
{
    z3::solver solver(*this->ctx);
    solver.add(this->condition && expr);

    z3::check_result result = solver.check();
    if (result == z3::check_result::sat)
    {
        model = solver.get_model().eval(indexer);
        return true;
    }
    else return result != z3::check_result::unsat;
}
