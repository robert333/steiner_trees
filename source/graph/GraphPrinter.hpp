#ifndef GRAPH_GRAPHPRINTER_HPP
#define GRAPH_GRAPHPRINTER_HPP

#include <string>

#include "Graph.hpp"

namespace graph {

class GraphPrinter {
public:

public:
	explicit GraphPrinter() = default;
	
	static void output(Graph const& graph);
	static void output(std::string const& filename, Graph const& graph);

	static void output_dimac(Graph const& graph);
	static void output_dimac(Graph const& graph, std::string const& filename);

	static void output(std::ostream& ostream, Graph const& graph);

private:
	static void output_edge(std::ostream& ostream, Edge const& edge, bool const& directed = true, bool const& weighted = true);

	static void output_dimac(std::ostream& ostream, Graph const& graph);

private:

};

} // namespace graph

#endif
