#include "SteinerTreeMIPFactory.hpp"

#include "natural_multi_commodity_flow/GroupEdges.hpp"
#include "natural_multi_commodity_flow/GroupMultiCommodityFlow.hpp"
#include "natural_multi_commodity_flow/GroupEdgesMultiCommodityFlow.hpp"
#include "natural_multi_commodity_flow/GroupSteinerTreeCuts.hpp"
#include "multi_commodity_dual/GroupMultiCommodityDual.hpp"

namespace steiner_trees {

mip::GroupManager SteinerTreeMIPFactory::create(
	SteinerTreeProblem const& steiner_tree_problem,
	SteinerTreeMIP::Type const& steiner_tree_mip_type
)
{
	switch (steiner_tree_mip_type) {
		case SteinerTreeMIP::NMC : return create_nmc(steiner_tree_problem);
		case SteinerTreeMIP::EMC : return create_nmc(steiner_tree_problem);
		case SteinerTreeMIP::EMC_DUAL : return create_emc_dual(steiner_tree_problem);
		case SteinerTreeMIP::CF : return create_nmc(steiner_tree_problem);
		case SteinerTreeMIP::UCB : return create_nmc(steiner_tree_problem);
		case SteinerTreeMIP::DCB : return create_dcb(steiner_tree_problem);
		default: FORBIDDEN;
	}
}

mip::GroupManager SteinerTreeMIPFactory::create_nmc(SteinerTreeProblem const& steiner_tree_problem)
{
	GroupEdges::SharedPtr group_edges(
		std::make_shared<GroupEdges>(
			"GroupEdges",
			steiner_tree_problem.graph(),
			steiner_tree_problem.nets()
		)
	);

	GroupMultiCommodityFlow::SharedPtr group_multi_commodity_flow(
		std::make_shared<GroupMultiCommodityFlow>(
			"GroupMultiCommodityFlow",
			steiner_tree_problem.graph(),
			steiner_tree_problem.nets()
		)
	);

	GroupEdgesMultiCommodityFlow::SharedPtr group_edges_multi_commodity_flow(
		std::make_shared<GroupEdgesMultiCommodityFlow>(
			"GroupEdgesMultiCommodityFlow",
			steiner_tree_problem.graph(),
			steiner_tree_problem.nets(),
			*group_edges,
			*group_multi_commodity_flow
		)
	);

	mip::GroupManager group_manager_steiner_tree_mip{};

	group_manager_steiner_tree_mip.add(group_edges);
	group_manager_steiner_tree_mip.add(group_multi_commodity_flow);
	group_manager_steiner_tree_mip.add(group_edges_multi_commodity_flow);

	return group_manager_steiner_tree_mip;
}

mip::GroupManager SteinerTreeMIPFactory::create_dcb(SteinerTreeProblem const& steiner_tree_problem)
{
	GroupEdges::SharedPtr group_edges(
		std::make_shared<GroupEdges>(
			"GroupEdges",
			steiner_tree_problem.graph(),
			steiner_tree_problem.nets()
		)
	);

	GroupSteinerTreeCuts::SharedPtr group_steiner_tree_cuts(
		std::make_shared<GroupSteinerTreeCuts>(
			"GroupSteinerTreeCuts",
			steiner_tree_problem.graph(),
			steiner_tree_problem.nets(),
			*group_edges
		)
	);

	mip::GroupManager group_manager_steiner_tree_mip{};

	group_manager_steiner_tree_mip.add(group_edges);
	group_manager_steiner_tree_mip.add(group_steiner_tree_cuts);

	return group_manager_steiner_tree_mip;
}

mip::GroupManager SteinerTreeMIPFactory::create_emc_dual(SteinerTreeProblem const& steiner_tree_problem)
{
	GroupMultiCommodityDual::SharedPtr group_multi_commodity_dual(
		std::make_shared<GroupMultiCommodityDual>(
			"GroupMultiCommodityDual",
			steiner_tree_problem.graph(),
			steiner_tree_problem.nets()
		)
	);

	mip::GroupManager group_manager_steiner_tree_mip{};

	group_manager_steiner_tree_mip.add(group_multi_commodity_dual);

	return group_manager_steiner_tree_mip;
}

} // namespace steiner_trees
