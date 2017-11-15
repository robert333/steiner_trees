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
	mip::MIP mip("SteinerTreeMIP", optimization_problem);

	mip::GroupManager const group_manager = YannickMIPFactory::create(yannick_problem);

	group_manager.create_variables_constraints_and_objective(mip);

	mip.output(Logger::logger());

	std::cout << "[SteinerTreeMIP] num variables   = " << mip.num_variables() << "\n";
	std::cout << "[SteinerTreeMIP] num constraints = " << mip.num_constraints() << "\n";
	std::cout << "[SteinerTreeMIP] optimize...\n";
	mip::MIP::Solver::ResultStatus const optimization_result = mip.optimize();

	if (optimization_result == mip::MIP::Solver::OPTIMAL) {
		mip::Solution const mip_solution = group_manager.compute_solution();

		return YannickSolution(
			optimization_problem,
			mip::MIP::MINIMIZATION,
			optimization_result,
			mip.objective_value(),
			{
				"task_time", mip_solution.get("GroupTaskTime"),
				"task_cycle", mip_solution.get("GroupTaskCycle"),
				"task_machine", mip_solution.get("GroupTaskMachine"),
			}
		);
	} else {
		assert(optimization_result == mip::MIP::Solver::INFEASIBLE);

		return YannickSolution(
			optimization_problem,
			mip::MIP::MINIMIZATION,
			optimization_result,
			0,
			{}
		);
	}
}

} // namespace yannick
