#include "YannickMIP.hpp"
#include "../mip/GroupManager.hpp"
#include "YannickMIPFactory.hpp"
#include "../Logger.hpp"

namespace yannick {

YannickSolution YannickMIP::solve(
	YannickProblem const& yannick_problem,
	mip::MIP::OptimizationProblem const& optimization_problem
)
{
	mip::MIP mip("YannickMIP", optimization_problem);

	mip::MIP::OptimizationType optimization_type = mip::MIP::MINIMIZATION;

	mip::GroupManager const group_manager = YannickMIPFactory::create(yannick_problem);

	group_manager.create_variables_constraints_and_objective(mip);

//	mip.output(Logger::logger());

	mip.output_statistics(std::cout);

	std::cout << "[YannickMIP] optimize...\n";
	mip::MIP::Solver::ResultStatus const optimization_result = mip.optimize();

	if (optimization_result == mip::MIP::Solver::OPTIMAL) {
		mip::Solution const mip_solution(
			optimization_problem,
			optimization_type,
			optimization_result,
			mip.objective_value(),
			group_manager.compute_solutions()
		);

		return YannickSolution(mip_solution);
	} else {
		assert(optimization_result == mip::MIP::Solver::INFEASIBLE);

		mip::Solution const mip_solution(
			optimization_problem,
			optimization_type,
			optimization_result,
			0,
			{}
		);

		return YannickSolution(mip_solution);
	}
}

} // namespace yannick
