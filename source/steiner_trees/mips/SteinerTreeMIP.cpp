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

	mip::MIP::OptimizationType optimization_type;

	switch (steiner_tree_mip_type) {
		case SteinerTreeMIP::NMC : FORBIDDEN;
		case SteinerTreeMIP::EMC : optimization_type = mip::MIP::MINIMIZATION; break;
		case SteinerTreeMIP::EMC_DUAL : optimization_type = mip::MIP::MAXIMIZATION; break;
		case SteinerTreeMIP::CF : FORBIDDEN;
		case SteinerTreeMIP::UCB : FORBIDDEN;
		case SteinerTreeMIP::DCB : optimization_type = mip::MIP::MINIMIZATION; break;
		case SteinerTreeMIP::DCB_DUAL : optimization_type = mip::MIP::MAXIMIZATION; break;
		case SteinerTreeMIP::SIMPLEX_EMBEDDING : optimization_type = mip::MIP::MAXIMIZATION; break;
		case SteinerTreeMIP::OPTIMAL_3_TERMINALS : optimization_type = mip::MIP::MAXIMIZATION; break;
		case SteinerTreeMIP::BIDIRECTED_MULTI_COMMODITY_COMMON_FLOW : optimization_type = mip::MIP::MINIMIZATION; break;
		default: FORBIDDEN;
	}

	mip.set_optimization_type(optimization_type);

	mip::GroupManager const group_manager = SteinerTreeMIPFactory::create(steiner_tree_problem, steiner_tree_mip_type);

	group_manager.create_variables_constraints_and_objective(mip);

	mip.output(Logger::logger());

	std::cout << "[SteinerTreeMIP] num variables   = " << mip.num_variables() << "\n";
	std::cout << "[SteinerTreeMIP] num constraints = " << mip.num_constraints() << "\n";
	std::cout << "[SteinerTreeMIP] optimize...\n";
	mip::MIP::Solver::ResultStatus const optimization_result = mip.optimize();

	if (optimization_result == mip::MIP::Solver::OPTIMAL) {
		mip::Solution const mip_solution(
			optimization_problem,
			optimization_type,
			optimization_result,
			mip.objective_value(),
			group_manager.compute_solutions()
		);

		return SteinerTreeSolution(mip_solution);
	} else {
		assert(optimization_result == mip::MIP::Solver::INFEASIBLE);

		mip::Solution const mip_solution(
			optimization_problem,
			optimization_type,
			optimization_result,
			0,
			{}
		);

		return SteinerTreeSolution(mip_solution);
	}
}

} // namespace steiner_trees
