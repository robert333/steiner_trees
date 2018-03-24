#ifndef STEINER_TREES_STEINERTREESOLVER_HPP
#define STEINER_TREES_STEINERTREESOLVER_HPP

#include "SteinerTreeProblem.hpp"
#include "SteinerTreeSolution.hpp"
#include "mips/SteinerTreeMIP.hpp"

namespace steiner_trees {

class SteinerTreeSolver {
public:
	using SharedPtr = std::shared_ptr<SteinerTreeSolver>;
	using Vector = std::vector<SteinerTreeSolver>;

public:
	static SteinerTreeSolution solve(
		SteinerTreeProblem const& steiner_tree_problem,
		SteinerTreeMIP::Type const& steiner_tree_mip_type,
		mip::MIP::OptimizationProblem const& optimization_problem
	);

private:
	explicit SteinerTreeSolver() = default;

private:
};

} // namespace steiner_trees

#endif
