#include "SkutellaMIP.hpp"
#include "../mip/GroupManager.hpp"
#include "SkutellaMIPFactory.hpp"
#include "../Logger.hpp"

namespace skutella {

SkutellaSolution SkutellaMIP::solve(
	SkutellaProblem const& skutella_problem,
	mip::MIP::OptimizationProblem const& optimization_problem
)
{
	mip::MIP mip("SkutellaMIP", optimization_problem);

	mip::MIP::OptimizationType const optimization_type = mip::MIP::MAXIMIZATION;

	mip.set_optimization_type(optimization_type);

	mip::GroupManager const group_manager = SkutellaMIPFactory::create(skutella_problem);

	group_manager.create_variables_constraints_and_objective(mip);

//	mip.output(Logger::logger());

	mip.output_statistics(std::cout);

	std::cout << "[SkutellaMIP] optimize...\n";
	mip::MIP::Solver::ResultStatus const optimization_result = mip.optimize();

	if (optimization_result == mip::MIP::Solver::OPTIMAL) {
		mip::Solution const mip_solution(
			optimization_problem,
			optimization_type,
			optimization_result,
			mip.objective_value(),
			group_manager.compute_solutions()
		);

		return SkutellaSolution(mip_solution);
	} else {
		assert(optimization_result == mip::MIP::Solver::INFEASIBLE);

		mip::Solution const mip_solution(
			optimization_problem,
			optimization_type,
			optimization_result,
			0,
			{}
		);

		return SkutellaSolution(mip_solution);
	}
}

} // namespace skutella
