#include "YannickSolution.hpp"

namespace yannick {

YannickSolution::YannickSolution(
	mip::MIP::OptimizationProblem const& optimization_problem,
	mip::MIP::OptimizationType const& optimization_type,
	mip::MIP::OptimizationResult const& optimization_result,
	mip::MIP::Value const& optimization_value,
	json const& optimization_solution
) :
	_optimization_problem(optimization_problem),
	_optimization_type(optimization_type),
	_optimization_result(optimization_result),
	_optimization_value(optimization_value),
	_optimization_solution(optimization_solution)
{}

mip::MIP::OptimizationProblem const& YannickSolution::optimization_problem() const
{
	return _optimization_problem;
}

mip::MIP::OptimizationType const& YannickSolution::optimization_type() const
{
	return _optimization_type;
}

mip::MIP::OptimizationResult const& YannickSolution::optimization_result() const
{
	return _optimization_result;
}

mip::MIP::Value const& YannickSolution::optimization_value() const
{
	return _optimization_value;
}

json const& YannickSolution::optimization_solution() const
{
	return _optimization_solution;
}

json YannickSolution::export_to_json() const
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

} // namespace yannick
