#ifndef STEINER_TREES_STEINERTREEMIP_HPP
#define STEINER_TREES_STEINERTREEMIP_HPP

#include "../SteinerTreeProblem.hpp"
#include "../SteinerTreeSolution.hpp"

namespace steiner_trees {

class SteinerTreeMIP {
public:
	enum Type {
		NMCF, // natural multi-commodity flow
		EMCF, // extended multi-commodity flow
		EMCF_DUAL, // dual of extended multi-commodity flow
		CF, // common-flow
		UCB, // undirected cut based
		DCB, // directed cut based
		DCB_DUAL, // bidirected cut based dual
		SIMPLEX_EMBEDDING, // lambda simplex embedding
		OPTIMAL_3_TERMINALS,
		BIDIRECTED_MULTI_COMMODITY_COMMON_FLOW
	};

	static mip::Solution solve(
		SteinerTreeProblem const& steiner_tree_problem,
		Type const& steiner_tree_mip_type,
		mip::MIP::OptimizationProblem const& optimization_problem
	);

public:
	explicit SteinerTreeMIP() = default;

private:
	static mip::MIP::OptimizationType optimization_type(Type const& type) {
		switch (type) {
			case SteinerTreeMIP::NMCF : FORBIDDEN;
			case SteinerTreeMIP::EMCF : return mip::MIP::MINIMIZATION;
			case SteinerTreeMIP::EMCF_DUAL : return mip::MIP::MAXIMIZATION;
			case SteinerTreeMIP::CF : FORBIDDEN;
			case SteinerTreeMIP::UCB : FORBIDDEN;
			case SteinerTreeMIP::DCB : return mip::MIP::MINIMIZATION;
			case SteinerTreeMIP::DCB_DUAL : return mip::MIP::MAXIMIZATION;
			case SteinerTreeMIP::SIMPLEX_EMBEDDING : return mip::MIP::MAXIMIZATION;
			case SteinerTreeMIP::OPTIMAL_3_TERMINALS : return mip::MIP::MAXIMIZATION;
			case SteinerTreeMIP::BIDIRECTED_MULTI_COMMODITY_COMMON_FLOW : return mip::MIP::MINIMIZATION;
			default: FORBIDDEN;
		}
	}
};

} // namespace steiner_trees

#endif
