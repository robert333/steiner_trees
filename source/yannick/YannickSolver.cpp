#include "YannickSolver.hpp"

#include "YannickMIP.hpp"

namespace yannick {

YannickSolution YannickSolver::solve(YannickProblem const& yannick_problem)
{
	std::cout << "[YannickSolver] solve...\n";
	return YannickMIP::solve(yannick_problem, mip::MIP::MIXED_INTEGER_PROGRAMMING);
}

} // namespace yannick
