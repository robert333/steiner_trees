#ifndef STEINER_TREES_STEINERTREEMIPFACTORY_HPP
#define STEINER_TREES_STEINERTREEMIPFACTORY_HPP

#include "../../mip/GroupManager.hpp"
#include "SteinerTreeMIP.hpp"

namespace steiner_trees {

class SteinerTreeMIPFactory {
public:
	static mip::GroupManager create(
		SteinerTreeProblem const& steiner_tree_problem,
		SteinerTreeMIP::Type const& steiner_tree_mip_type
	);

	static mip::GroupManager create_emc(SteinerTreeProblem const& steiner_tree_problem);
	static mip::GroupManager create_dcb(SteinerTreeProblem const& steiner_tree_problem);
	static mip::GroupManager create_emc_dual(SteinerTreeProblem const& steiner_tree_problem);

	static mip::GroupManager create_simplex_embedding(SteinerTreeProblem const& steiner_tree_problem);

private:

};

} // namespace steiner_trees

#endif
