#include "SteinerTreeSolution.hpp"

namespace steiner_trees {

SteinerTreeSolution::SteinerTreeSolution(
	graph::Weight const& optimum_value,
	json const& solution
) :
	_optimum_value(optimum_value),
	_solution(solution)
{}

graph::Weight const& SteinerTreeSolution::optimum_value() const
{
	return _optimum_value;
}

json const& SteinerTreeSolution::solution() const
{
	return _solution;
}

} // namespace steiner_trees
