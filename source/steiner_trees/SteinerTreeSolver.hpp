#ifndef STEINER_TREES_STEINERTREESOLVER_HPP
#define STEINER_TREES_STEINERTREESOLVER_HPP

#include "SteinerTreeProblem.hpp"
#include "SteinerTreeSolution.hpp"

namespace steiner_trees {

class SteinerTreeSolver {
public:
	using SharedPtr = std::shared_ptr<SteinerTreeSolver>;
	using Vector = std::vector<SteinerTreeSolver>;

public:
	static SteinerTreeSolution solve(
		SteinerTreeProblem const& steiner_tree_problem,
		mip::MIP::OptimizationProblem const& optimization_problem
	);

private:
	explicit SteinerTreeSolver() = default;

private:
	std::string _name;
	graph::Graph _graph;
	graph::Net _net;
};

} // namespace steiner_trees

#endif
