#include "SteinerTreeSolver.hpp"

#include <steiner_trees/dijkstra_steiner/DijkstraSteiner.hpp>
#include <Chronograph.hpp>

namespace steiner_trees {

SteinerTreeSolution SteinerTreeSolver::solve_via_combinatorial_optimization(
	SteinerTreeProblem const& steiner_tree_problem
)
{
	std::cout << "[SteinerTreeSolver] Solve Steiner Tree problem via combinatorial optimization\n";

	Chronograph const chronograph("Steiner Tree DijkstraSteiner Solver");

	return DijkstraSteiner::solve(steiner_tree_problem);
}

SteinerTreeSolution SteinerTreeSolver::solve_via_linear_programming(
	SteinerTreeProblem const& steiner_tree_problem,
	SteinerTreeMIP::Type const& steiner_tree_mip_type,
	mip::MIP::OptimizationProblem const& optimization_problem
)
{
	std::cout << "[SteinerTreeSolver] Solve Steiner Tree problem via linear programming\n";
	std::cout << "[SteinerTreeSolver] Steiner Tree MIP type = " << steiner_tree_mip_type << "\n";
	std::cout << "[SteinerTreeSolver] MIP problem type      = " << optimization_problem << "\n";

	Chronograph const chronograph("Steiner Tree LP/MIP Solver");

	mip::Solution const mip_solution = SteinerTreeMIP::solve(
		steiner_tree_problem,
		steiner_tree_mip_type,
		optimization_problem
	);

	assert(mip_solution.optimization_result() == mip::MIP::Solver::OPTIMAL);

	return SteinerTreeSolution(
		mip_solution.optimization_value(),
		mip_solution.export_to_json()
	);
}

} // namespace steiner_trees
