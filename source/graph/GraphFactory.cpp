// GraphFactory.cpp
#include "GraphFactory.hpp"

namespace graph {

Graph GraphFactory::create_from_json(json const& graph_json)
{
	Graph graph(static_cast<bool>(graph_json["directed"]));

	for (json const& node : graph_json["nodes"]) {
		Weight const weight = node.count("weight") == 1 ? static_cast<Weight>(node["weight"]) : 0;
		NodeId const node_id = graph.create_node(weight);
		assert(node["id"] == node_id);
	}

	for (json const& edge : graph_json["edges"]) {
		Weight const weight = edge.count("weight") == 1 ? static_cast<Weight>(edge["weight"]) : 1;
		graph.create_edge(edge["tail"], edge["head"], weight);
	}

	return graph;
}

} // namespace graph
