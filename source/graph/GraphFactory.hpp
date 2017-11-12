#ifndef GRAPH_GRAPHFACTORY_HPP
#define GRAPH_GRAPHFACTORY_HPP

#include "Graph.hpp"
#include "../json.hpp"

namespace graph {

class GraphFactory {
public:

public:
	static Graph create_from_json(json const& graph_json);
	static Graph input(std::string const& filename, bool const& directed=false, bool const& weighted=false);

private:
	explicit GraphFactory() = default;

private:

};

} // namespace graph

#endif
