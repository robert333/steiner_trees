#ifndef GRAPH_MOOREBELLMANFORD_HPP
#define GRAPH_MOOREBELLMANFORD_HPP

#include "../Graph.hpp"

namespace graph {

class MooreBellmanFord {
public:
	static Weight compute_longest_path(Graph const& graph, Node const& source, Node const& target);

public:
	static std::deque<NodeId> compute(Graph const& graph, Node const& source, std::vector<Weight>& length);

private:
	explicit MooreBellmanFord() = default;
};

} // namespace graph

#endif
