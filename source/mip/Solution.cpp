#include "Solution.hpp"

#include "../helper/map.hpp"

namespace mip {

Solution::Solution(
	MIP::OptimizationProblem const& optimization_problem,
	MIP::OptimizationType const& optimization_type,
	MIP::OptimizationResult const& optimization_result,
	Value const& optimization_value,
	json const& optimization_solution
) :
	_optimization_problem(optimization_problem),
	_optimization_type(optimization_type),
	_optimization_result(optimization_result),
	_optimization_value(optimization_value),
	_optimization_solution(optimization_solution)
{}

MIP::OptimizationProblem const& Solution::optimization_problem() const
{
	return _optimization_problem;
}

MIP::OptimizationType const& Solution::optimization_type() const
{
	return _optimization_type;
}

MIP::OptimizationResult const& Solution::optimization_result() const
{
	return _optimization_result;
}

Value const& Solution::optimization_value() const
{
	return _optimization_value;
}

json const& Solution::optimization_solution() const
{
	return _optimization_solution;
}

json Solution::export_to_json() const
{
	return {
		{
			"variables",
			optimization_solution()
		},
		{
			"optimization",
			{
				{"problem", mip::MIP::to_string(optimization_problem())},
				{"type", mip::MIP::to_string(optimization_type())},
				{"objective_value", optimization_value()}
			}
		}
	};
}

} // namespace mip
