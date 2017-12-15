#include "SteinerTreeSolver.hpp"

#include "mips/SteinerTreeMIP.hpp"

namespace steiner_trees {

SteinerTreeSolution SteinerTreeSolver::solve(
	SteinerTreeProblem const& steiner_tree_problem,
	mip::MIP::OptimizationProblem const& optimization_problem
)
{
	std::cout << "[SteinerTreeSolver] optimize...\n";
	return SteinerTreeMIP::solve(steiner_tree_problem, SteinerTreeMIP::SIMPLEX_EMBEDDING, optimization_problem);
}

} // namespace steiner_trees
