// GraphPrinter.cpp
#include "GraphPrinter.hpp"

#include <fstream>
#include <sstream>

namespace graph {

void GraphPrinter::output(Graph const& graph)
{
	output(std::cout, graph);
}

void GraphPrinter::output(std::string const& filename, Graph const& graph)
{
	std::ofstream file(filename);
	assert(file.is_open());
	output(file, graph);
	file.close();
}

void GraphPrinter::output_dimac(Graph const &graph) {
	output_dimac(std::cout, graph);
}

void GraphPrinter::output_dimac(Graph const& graph, std::string const& filename)
{
	std::ofstream file(filename);

	if (not file.is_open()) {
		throw;
	}

	output_dimac(file, graph);

	file.close();
}

void GraphPrinter::output(std::ostream& ostream, Graph const& graph)
{
	std::size_t const num_nodes = graph.num_nodes();
	std::size_t const num_edges = graph.num_edges();
	
	bool const directed = graph.is_directed();
	bool const weighted = graph.is_weighted();

	if (directed) {
		ostream << "Directed ";
	}
	
	if (weighted) {
		ostream << "Weighted ";
	}
	
	ostream << "Graph with " << num_nodes << " nodes and " << num_edges << " edges\n\n";

	for (NodeId node_id = 0; node_id < num_nodes; ++node_id) {
		Node const& node = graph.node(node_id);

		if (directed) {
			ostream << "Node " << node_id << " with "
				    << node.num_incoming_edges() << " incoming edges and "
				    << node.num_outgoing_edges() << " outgoing edges\n";

			for (EdgeId const edge_id : node.incoming_edges()) {
				Edge const& edge = graph.edge(edge_id);
				output_edge(ostream, edge, directed, weighted);
			}

			for (EdgeId const edge_id : node.outgoing_edges()) {
				Edge const& edge = graph.edge(edge_id);
				output_edge(ostream, edge, directed, weighted);
			}
		} else {
			ostream << "Node " << node_id << " with " << node.num_incident_edges() << " incident edges\n";

			for (EdgeId const edge_id : node.incident_edges()) {
				Edge const& edge = graph.edge(edge_id);
				output_edge(ostream, edge, directed, weighted);
			}
		}

		ostream << "\n";
	}
	
	ostream << "Edges\n";
	
	for (EdgeId edge_id = 0; edge_id < num_edges; ++edge_id) {
		Edge const& edge = graph.edge(edge_id);
		output_edge(ostream, edge, directed, weighted);
	}
}

void GraphPrinter::output_edge(std::ostream& ostream, Edge const& edge, bool const& directed, bool const& weighted)
{
	ostream << "[" << edge.id() << "] = "
			<< edge.tail() << " --";
	
	if (weighted) {
		ostream << "( " << edge.weight() << " )";
	}
	
	ostream << "--";
	
	if (directed) {
		ostream << ">";
	}
	
	ostream << " " << edge.head() << "\n";
}

void GraphPrinter::output_dimac(std::ostream& ostream, Graph const& graph) {
	ostream << "p edge " << graph.num_nodes() << " " << graph.num_edges() << "\n";

	for (Edge const& edge : graph.edges()) {
		ostream << "e " << edge.tail() + 1 << " " << edge.head() + 1 << "\n";
	}
}

} // namespace graph
