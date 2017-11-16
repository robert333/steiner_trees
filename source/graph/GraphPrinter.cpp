#include "GraphPrinter.hpp"

#include <fstream>

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
	if (graph.is_directed()) {
		ostream << "Directed ";
	}
	
	ostream << "Graph with " << graph.num_nodes() << " nodes and " << graph.num_edges() << " edges\n\n";

	for (Node const& node : graph.nodes()) {
		ostream << "Node " << node << " with weight " << node.weight() << " and ";

		if (graph.is_directed()) {
			ostream << node.num_incoming_edges() << " incoming edges and "
				    << node.num_outgoing_edges() << " outgoing edges\n";

			ostream << "incoming:\n";

			for (EdgeId const& edge_id : node.incoming_edges()) {
				Edge const& edge = graph.edge(edge_id);
				output_edge(ostream, edge, graph.is_directed());
			}

			ostream << "outgoing:\n";

			for (EdgeId const& edge_id : node.outgoing_edges()) {
				Edge const& edge = graph.edge(edge_id);
				output_edge(ostream, edge, graph.is_directed());
			}
		} else {
			ostream << node.num_incident_edges() << " incident edges\n";

			for (EdgeId const& edge_id : node.incident_edges()) {
				Edge const& edge = graph.edge(edge_id);
				output_edge(ostream, edge, graph.is_directed());
			}
		}

		ostream << "\n";
	}

	ostream << "Nodes\n";

	for (Node const& node : graph.nodes()) {
		output_node(ostream, node);
	}

	ostream << "\n";
	
	ostream << "Edges\n";
	
	for (Edge const& edge : graph.edges()) {
		output_edge(ostream, edge, graph.is_directed());
	}
}

void GraphPrinter::output_node(std::ostream& ostream, Node const& node)
{
	ostream << "[" << node.id() << "] = (weight = " << node.weight() << ")\n";
}

void GraphPrinter::output_edge(std::ostream& ostream, Edge const& edge, bool directed)
{
	ostream << "[" << edge.id() << "] = "
			<< edge.tail() << " --";
	
	ostream << "( " << edge.weight() << " )";
	
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
