#include "SteinerTreeMIPFactory.hpp"

#include "natural_multi_commodity_flow/GroupEdges.hpp"
#include "natural_multi_commodity_flow/GroupMultiCommodityFlow.hpp"
#include "natural_multi_commodity_flow/GroupEdgesMultiCommodityFlow.hpp"
#include "natural_multi_commodity_flow/GroupSteinerTreeCuts.hpp"
#include "multi_commodity_dual/GroupMultiCommodityDual.hpp"
#include "simplex_embedding/GroupSimplexEmbedding.hpp"

namespace steiner_trees {

mip::GroupManager SteinerTreeMIPFactory::create(
	SteinerTreeProblem const& steiner_tree_problem,
	SteinerTreeMIP::Type const& steiner_tree_mip_type
)
{
	switch (steiner_tree_mip_type) {
		case SteinerTreeMIP::NMC : FORBIDDEN;
		case SteinerTreeMIP::EMC : return create_emc(steiner_tree_problem);
		case SteinerTreeMIP::EMC_DUAL : return create_emc_dual(steiner_tree_problem);
		case SteinerTreeMIP::CF : FORBIDDEN;
		case SteinerTreeMIP::UCB : FORBIDDEN;
		case SteinerTreeMIP::DCB : return create_dcb(steiner_tree_problem);
		case SteinerTreeMIP::SIMPLEX_EMBEDDING : return create_simplex_embedding(steiner_tree_problem);
		default: FORBIDDEN;
	}
}

mip::GroupManager SteinerTreeMIPFactory::create_emc(SteinerTreeProblem const& steiner_tree_problem)
{
	GroupEdges::SharedPtr group_edges(
		std::make_shared<GroupEdges>(
			"GroupEdges",
			steiner_tree_problem.graph().bidirect(),
			steiner_tree_problem.nets()
		)
	);

	GroupMultiCommodityFlow::SharedPtr group_multi_commodity_flow(
		std::make_shared<GroupMultiCommodityFlow>(
			"GroupMultiCommodityFlow",
			steiner_tree_problem.graph().bidirect(),
			steiner_tree_problem.nets()
		)
	);

	GroupEdgesMultiCommodityFlow::SharedPtr group_edges_multi_commodity_flow(
		std::make_shared<GroupEdgesMultiCommodityFlow>(
			"GroupEdgesMultiCommodityFlow",
			*group_edges,
			*group_multi_commodity_flow
		)
	);

	mip::GroupManager group_manager{};

	group_manager.add(group_edges);
	group_manager.add(group_multi_commodity_flow);
	group_manager.add(group_edges_multi_commodity_flow);

	return group_manager;
}

mip::GroupManager SteinerTreeMIPFactory::create_dcb(SteinerTreeProblem const& steiner_tree_problem)
{
	GroupEdges::SharedPtr group_edges(
		std::make_shared<GroupEdges>(
			"GroupEdges",
			steiner_tree_problem.graph().bidirect(),
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

} // namespace steiner_trees
