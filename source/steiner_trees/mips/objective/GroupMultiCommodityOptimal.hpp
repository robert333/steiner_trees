#ifndef STEINER_TREES_GROUPMULTICOMMODITYOPTIMAL_HPP
#define STEINER_TREES_GROUPMULTICOMMODITYOPTIMAL_HPP

#include "../natural_multi_commodity_flow/GroupEdges.hpp"
#include "../multi_commodity_dual/GroupMultiCommodityDual.hpp"

namespace steiner_trees {

class GroupMultiCommodityOptimal : public mip::Group {
public:
	using SharedPtr = std::shared_ptr<GroupMultiCommodityOptimal>;

public:
	explicit GroupMultiCommodityOptimal(
		std::string const& name,
		GroupEdges const& group_edges,
		GroupMultiCommodityDual const& group_multi_commodity_dual
	);

	void create_variables_constraints_and_objective(mip::MIPModel& mip_model) final;

private:
	void create_constraints(mip::MIPModel& mip_model);

private:
	GroupEdges const& _group_edges;
	GroupMultiCommodityDual const& _group_multi_commodity_dual;
};

} // namespace steiner_trees

#endif
