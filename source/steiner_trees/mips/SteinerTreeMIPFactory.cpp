#include "SteinerTreeMIPFactory.hpp"

#include "natural_multi_commodity_flow/GroupEdges.hpp"
#include "natural_multi_commodity_flow/GroupMultiCommodityFlow.hpp"
#include "natural_multi_commodity_flow/GroupSteinerTreeCuts.hpp"
#include "multi_commodity_dual/GroupMultiCommodityDual.hpp"
#include "simplex_embedding/GroupSimplexEmbedding.hpp"
#include "natural_multi_commodity_flow/GroupCommonFlow.hpp"
#include "natural_multi_commodity_flow/GroupDynamicGraph.hpp"
#include "objective/GroupMultiCommodityOptimal.hpp"
#include "bidirected_cuts_dual/GroupBidirectedCutsDual.hpp"
#include "groups/GroupBidirectedMultiCommodityCommonFlow.hpp"

namespace steiner_trees {

mip::GroupManager SteinerTreeMIPFactory::create(
	SteinerTreeProblem const& steiner_tree_problem,
	SteinerTreeMIP::Type const& steiner_tree_mip_type
)
{
	switch (steiner_tree_mip_type) {
		case SteinerTreeMIP::NMCF : FORBIDDEN;
		case SteinerTreeMIP::EMCF : return create_emc(steiner_tree_problem);
		case SteinerTreeMIP::EMCF_DUAL : return create_emc_dual(steiner_tree_problem);
		case SteinerTreeMIP::CF : FORBIDDEN;
		case SteinerTreeMIP::UCB : FORBIDDEN;
		case SteinerTreeMIP::DCB : return create_dcb(steiner_tree_problem);
		case SteinerTreeMIP::DCB_DUAL : return create_dcb_dual(steiner_tree_problem);
		case SteinerTreeMIP::SIMPLEX_EMBEDDING : return create_simplex_embedding(steiner_tree_problem);
		case SteinerTreeMIP::OPTIMAL_3_TERMINALS : return create_optimal_3_terminals(steiner_tree_problem);
		case SteinerTreeMIP::BIDIRECTED_MULTI_COMMODITY_COMMON_FLOW : return create_bidirected_multi_commodity_common_flow(steiner_tree_problem);
		default: FORBIDDEN;
	}
}

mip::GroupManager SteinerTreeMIPFactory::create_emc(SteinerTreeProblem const& steiner_tree_problem)
{
	GroupEdges::SharedPtr group_edges(
		std::make_shared<GroupEdges>(
			"GroupEdges",
			steiner_tree_problem.terminal_instance(),
			steiner_tree_problem.nets()
		)
	);

	GroupMultiCommodityFlow::SharedPtr group_multi_commodity_flow(
		std::make_shared<GroupMultiCommodityFlow>(
			"GroupMultiCommodityFlow",
			*group_edges
		)
	);

	mip::GroupManager group_manager{};

	group_manager.add(group_edges);
	group_manager.add(group_multi_commodity_flow);

	return group_manager;
}

mip::GroupManager SteinerTreeMIPFactory::create_dcb(SteinerTreeProblem const& steiner_tree_problem)
{
	GroupEdges::SharedPtr group_edges(
		std::make_shared<GroupEdges>(
			"GroupEdges",
			steiner_tree_problem.terminal_instance(),
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

mip::GroupManager SteinerTreeMIPFactory::create_emc_dual(SteinerTreeProblem const& /*steiner_tree_problem*/)
{
//	GroupMultiCommodityDual::SharedPtr group_multi_commodity_dual(
//		std::make_shared<GroupMultiCommodityDual>(
//			"GroupMultiCommodityDual",
//			steiner_tree_problem.graph(),
//			steiner_tree_problem.nets()
//		)
//	);

	mip::GroupManager group_manager_steiner_tree_mip{};

//	group_manager_steiner_tree_mip.add(group_multi_commodity_dual);

	return group_manager_steiner_tree_mip;
}

	mip::GroupManager SteinerTreeMIPFactory::create_dcb_dual(SteinerTreeProblem const &steiner_tree_problem) {
		GroupBidirectedCutsDual::SharedPtr group_bidirected_cuts_dual(
				std::make_shared<GroupBidirectedCutsDual>(
						"GroupBidirectedCutsDual",
						steiner_tree_problem.graph(),
						steiner_tree_problem.nets()
				)
		);

		mip::GroupManager group_manager{};

		group_manager.add(group_bidirected_cuts_dual);

		return group_manager;
	}

mip::GroupManager SteinerTreeMIPFactory::create_simplex_embedding(SteinerTreeProblem const& steiner_tree_problem)
{
	assert(steiner_tree_problem.nets().size() == 1);

	mip::GroupManager group_manager{};

	group_manager.add(
		std::make_shared<GroupSimplexEmbedding>(
			"GroupSimplexEmbedding",
			steiner_tree_problem.graph(),
			steiner_tree_problem.nets().front()
		)
	);

	return group_manager;
}

mip::GroupManager SteinerTreeMIPFactory::create_optimal_3_terminals(SteinerTreeProblem const& steiner_tree_problem)
{
	assert(steiner_tree_problem.nets().size() == 1);

	GroupEdges::SharedPtr group_edges(
		std::make_shared<GroupEdges>(
			"GroupEdges",
			steiner_tree_problem.terminal_instance(),
			steiner_tree_problem.nets(),
			false,
			false
		)
	);

	GroupDynamicGraph::SharedPtr group_dynamic_graph(
		std::make_shared<GroupDynamicGraph>(
			"GroupDynamicGraph",
			*group_edges
		)
	);

	GroupMultiCommodityFlow::SharedPtr group_multi_commodity_flow(
		std::make_shared<GroupMultiCommodityFlow>(
			"GroupMultiCommodityFlow",
			*group_edges,
			false
		)
	);

	GroupMultiCommodityDual::SharedPtr group_multi_commodity_dual(
		std::make_shared<GroupMultiCommodityDual>(
			"GroupMultiCommodityDual",
			steiner_tree_problem.graph(),
			steiner_tree_problem.nets(),
			*group_dynamic_graph,
			false
		)
	);

	GroupMultiCommodityOptimal::SharedPtr group_multi_commodity_optimal(
		std::make_shared<GroupMultiCommodityOptimal>(
			"GroupMultiCommodityOptimal",
			*group_edges,
			*group_multi_commodity_dual
		)
	);

	GroupCommonFlow::SharedPtr group_common_flow(
		std::make_shared<GroupCommonFlow>(
			"GroupCommonFlow",
			*group_edges,
			*group_multi_commodity_flow,
			steiner_tree_problem.nets().front()
		)
	);

	mip::GroupManager group_manager{};

	group_manager.add(group_edges);
	group_manager.add(group_dynamic_graph);
	group_manager.add(group_multi_commodity_flow);
	group_manager.add(group_multi_commodity_dual);
	group_manager.add(group_multi_commodity_optimal);
	group_manager.add(group_common_flow);

	return group_manager;
}

mip::GroupManager SteinerTreeMIPFactory::create_bidirected_multi_commodity_common_flow(
	SteinerTreeProblem const& steiner_tree_problem
)
{
	assert(steiner_tree_problem.nets().size() == 1);

	GroupEdges::SharedPtr group_edges(
		std::make_shared<GroupEdges>(
			"GroupEdges",
			steiner_tree_problem.terminal_instance(),
			steiner_tree_problem.nets()
		)
	);

	GroupBidirectedMultiCommodityCommonFlow::SharedPtr group_multi_commodity_common_flow(
		std::make_shared<GroupBidirectedMultiCommodityCommonFlow>(
			"GroupBidirectedMultiCommodityCommonFlow",
			*group_edges
		)
	);

	mip::GroupManager group_manager{};

	group_manager.add(group_edges);
	group_manager.add(group_multi_commodity_common_flow);

	return group_manager;
}

} // namespace steiner_trees
