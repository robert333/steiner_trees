#include "MIPSolver.hpp"
#include "../Logger.hpp"

namespace mip {

MIPSolver::Solver::OptimizationProblemType MIPSolver::get_solver_optimization_problem_type(
	OptimizationProblem const& optimization_problem
)
{
	switch (optimization_problem) {
		case LINEAR_PROGRAMMING : return Solver::GLOP_LINEAR_PROGRAMMING;
		case MIXED_INTEGER_PROGRAMMING : return Solver::CBC_MIXED_INTEGER_PROGRAMMING;
		default: assert(false);
	}
}

std::string MIPSolver::to_string(OptimizationProblem const& optimization_problem)
{
	switch (optimization_problem) {
		case LINEAR_PROGRAMMING : return "LP";
		case MIXED_INTEGER_PROGRAMMING : return "MIP";
		default: assert(false);
	}
}

std::string MIPSolver::to_string(OptimizationType const& optimization_type)
{
	switch (optimization_type) {
		case MINIMIZATION : return "min";
		case MAXIMIZATION : return "max";
		default: assert(false);
	}
}

MIPSolver::MIPSolver(std::string const& name, OptimizationProblem const& optimization_problem) :
	_solver(std::make_shared<Solver>(name, get_solver_optimization_problem_type(optimization_problem)))
{
	solver().EnableOutput();
}

void MIPSolver::set_optimization_type(MIPSolver::OptimizationType const& optimization_type)
{
	switch (optimization_type) {
		case MINIMIZATION : solver().MutableObjective()->SetMinimization(); break;
		case MAXIMIZATION : solver().MutableObjective()->SetMaximization(); break;
	}
}

MIPSolver::Solver::ResultStatus MIPSolver::optimize()
{
	Logger::logger() << "[MIPSolver] number of variables   = " << solver().NumVariables() << "\n";
	Logger::logger() << "[MIPSolver] number of constraints = " << solver().NumConstraints() << "\n";

	Solver::ResultStatus const result_status = solver().Solve();

	if (result_status != Solver::OPTIMAL) {
		Logger::logger() << "[MIPSolver] The problem does not have an optimal solution!\n";
	}

	Logger::logger() << "[MIPSolver] Problem solved in " << solver().wall_time() << " milliseconds.\n";
	Logger::logger() << "[MIPSolver] optimal objective value = " << solver().Objective().Value() << "\n";

	return result_status;
}

MIPSolver::Solver& MIPSolver::solver()
{
	return *_solver;
}

MIPSolver::Solver const& MIPSolver::solver() const
{
	return *_solver;
}

} // namespace mip
