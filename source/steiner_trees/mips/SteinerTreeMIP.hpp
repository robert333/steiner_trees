#ifndef STEINER_TREES_STEINERTREEMIP_HPP
#define STEINER_TREES_STEINERTREEMIP_HPP

#include "../SteinerTreeProblem.hpp"
#include "../SteinerTreeSolution.hpp"

namespace steiner_trees {

class SteinerTreeMIP {
public:
	enum Type {
		NMC, // natural multi-commodity flow
		EMC, // extended multi-commodity flow
		EMC_DUAL, // dual of extended multi-commodity flow
		CF, // common-flow
		UCB, // undirected cut based
		DCB, // directed cut based
		SIMPLEX_EMBEDDING // lambda simplex embedding
	};

//	static SteinerTreeMIP create(
//		SteinerTreeProblem const& steiner_tree_problem,
//		Type const& steiner_tree_mip_type,
//		mip::MIP::OptimizationProblem const& optimization_problem
//	);

	static SteinerTreeSolution solve(
		SteinerTreeProblem const& steiner_tree_problem,
		Type const& steiner_tree_mip_type,
		mip::MIP::OptimizationProblem const& optimization_problem
	);

public:
	explicit SteinerTreeMIP() = default;

private:
};

} // namespace steiner_trees

#endif
