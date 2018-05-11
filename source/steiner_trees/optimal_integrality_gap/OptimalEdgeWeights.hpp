#ifndef STEINER_TREES_OPTIMALEDGEWEIGHTS_HPP
#define STEINER_TREES_OPTIMALEDGEWEIGHTS_HPP

#include <graph/TerminalInstance.hpp>
#include <mip/MIPModel.hpp>
#include <mip/VariableStorage.hpp>

namespace steiner_trees {

class OptimalEdgeWeights {
public:


private:
	static std::string variable_prefix() {
		return "OptimalEdgeWeights";
	}

	void create_edge_weight_variables(mip::MIPModel& mip_model, graph::Edge::Vector const& edges)
	{
		for (graph::Edge const& edge : edges) {
			mip::MIPModel::Variable* const variable = mip_model.create_continuous_variable(
				variable_prefix(),
				"weight for edge " + edge.to_string(),
				0, 1
			);

			_common_flow_variables.set(edge.id(), terminal_set, variable);
		}
	}

	void add_lower_bound(mip::MIPModel& mip_model, graph::Edge::Vector const& edges)
	{

	}


private:
	explicit OptimalEdgeWeights() = default;

private:
	mip::VariableStorage<graph::EdgeId> _edge_weight_variables;
};

} // namespace steiner_trees

#endif
