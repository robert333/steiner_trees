#include "SteinerTreeMIP.hpp"

#include "../../mip/GroupManager.hpp"
#include "SteinerTreeMIPFactory.hpp"

namespace steiner_trees {

mip::Solution SteinerTreeMIP::solve(
	SteinerTreeProblem const& steiner_tree_problem,
	SteinerTreeMIP::Type const& steiner_tree_mip_type,
	mip::MIP::OptimizationProblem const& optimization_problem)
{
	mip::MIP mip("SteinerTreeMIP", optimization_problem);

	mip.set_optimization_type(optimization_type(steiner_tree_mip_type));

	mip::GroupManager const group_manager = SteinerTreeMIPFactory::create(
		steiner_tree_problem,
		steiner_tree_mip_type
	);

	group_manager.create_variables_constraints_and_objective(mip);

	std::cout << "[SteinerTreeMIP] num variables   = " << mip.num_variables() << "\n";
	std::cout << "[SteinerTreeMIP] num constraints = " << mip.num_constraints() << "\n";
	std::cout << "[SteinerTreeMIP] optimize...\n";

	mip::MIP::Solver::ResultStatus const optimization_result = mip.optimize();

	assert(optimization_result == mip::MIP::Solver::OPTIMAL);

	return mip::Solution(
		optimization_problem,
		optimization_type(steiner_tree_mip_type),
		optimization_result,
		mip.objective_value(),
		group_manager.compute_solutions()
	);
}

} // namespace steiner_trees
