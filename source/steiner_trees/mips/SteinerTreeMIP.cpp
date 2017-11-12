#include "SteinerTreeMIP.hpp"
#include "../../mip/GroupManager.hpp"
#include "SteinerTreeMIPFactory.hpp"
#include "../../Logger.hpp"

namespace steiner_trees {

//SteinerTreeMIP SteinerTreeMIP::create(
//	SteinerTreeProblem const& steiner_tree_problem,
//	SteinerTreeMIP::Type const& steiner_tree_mip_type,
//	mip::MIP::OptimizationProblem const& optimization_problem
//)
//{
//	mip::MIP mip("SteinerTreeMIP", optimization_problem);
//
//	mip::GroupManager const group_manager = SteinerTreeMIPFactory::create(steiner_tree_problem, steiner_tree_mip_type);
//
//	group_manager.create_variables_constraints_and_objective(mip);
//
//	return SteinerTreeMIP(mip);
//}

SteinerTreeSolution SteinerTreeMIP::solve(
	SteinerTreeProblem const& steiner_tree_problem,
	SteinerTreeMIP::Type const& steiner_tree_mip_type,
	mip::MIP::OptimizationProblem const& optimization_problem)
{
	mip::MIP mip("SteinerTreeMIP", optimization_problem);

	mip::GroupManager const group_manager = SteinerTreeMIPFactory::create(steiner_tree_problem, steiner_tree_mip_type);

	group_manager.create_variables_constraints_and_objective(mip);

//	mip.output(Logger::logger());

	std::cout << "[SteinerTreeMIP] num variables   = " << mip.num_variables() << "\n";
	std::cout << "[SteinerTreeMIP] num constraints = " << mip.num_constraints() << "\n";
	std::cout << "[SteinerTreeMIP] optimize...\n";
	mip::MIP::Solver::ResultStatus const optimization_result = mip.optimize();

	if (optimization_result == mip::MIP::Solver::OPTIMAL) {
		mip::Solution const mip_solution = group_manager.compute_solution();

		return SteinerTreeSolution(
			optimization_problem,
			mip::MIP::MINIMIZATION,
			optimization_result,
			mip.objective_value(),
			mip_solution.get("GroupEdges")
		);
	} else {
		assert(optimization_result == mip::MIP::Solver::INFEASIBLE);

		return SteinerTreeSolution(
			optimization_problem,
			mip::MIP::MINIMIZATION,
			optimization_result,
			0,
			{}
		);
	}
}

SteinerTreeMIP::SteinerTreeMIP(mip::MIP const& mip) :
	_mip(mip)
{

}

} // namespace steiner_trees
