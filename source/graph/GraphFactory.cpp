// GraphFactory.cpp
#include "GraphFactory.hpp"

#include <fstream>

namespace graph {

Graph GraphFactory::create_from_json(json const& graph_json)
{
	std::size_t num_nodes = 0;

	for (json const& edge : graph_json["edges"]) {
		num_nodes = std::max({edge["tail"].get<NodeId>() + 1, edge["head"].get<NodeId>() + 1, num_nodes});
	}

	Graph graph(graph_json["directed"], num_nodes);

	for (json const& edge : graph_json["edges"]) {
		Weight const weight = graph_json["weighted"] ? static_cast<Weight>(edge["weight"]) : 1;
		graph.create_edge(edge["tail"], edge["head"], weight);
	}

	return graph;
}

} // namespace graph
