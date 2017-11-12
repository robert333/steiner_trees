#include "SteinerTreeSolution.hpp"

namespace steiner_trees {

SteinerTreeSolution::SteinerTreeSolution(
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

mip::MIP::OptimizationProblem const& SteinerTreeSolution::optimization_problem() const
{
	return _optimization_problem;
}

mip::MIP::OptimizationType const& SteinerTreeSolution::optimization_type() const
{
	return _optimization_type;
}

mip::MIP::OptimizationResult const& SteinerTreeSolution::optimization_result() const
{
	return _optimization_result;
}

mip::MIP::Value const& SteinerTreeSolution::optimization_value() const
{
	return _optimization_value;
}

json const& SteinerTreeSolution::optimization_solution() const
{
	return _optimization_solution;
}

json SteinerTreeSolution::export_to_json() const
{
	return {
		{
			"edges",
			optimization_solution()["edges"]
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

} // namespace steiner_trees
