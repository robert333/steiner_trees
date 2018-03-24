#ifndef GRAPH_TERMINALINSTANCE_HPP
#define GRAPH_TERMINALINSTANCE_HPP

#include "Graph.hpp"

namespace graph {

class TerminalInstance {
public:
	using SharedPtr = std::shared_ptr<TerminalInstance>;
	using Vector = std::vector<TerminalInstance>;

public:
	explicit TerminalInstance(
		Graph const& undirected_graph,
		std::vector<NodeId> const& terminals
	);

	Graph const& undirected_graph() const;
	Graph const& bidirected_graph() const;
	std::vector<NodeId> const& terminals() const;

	NodeId const& terminal(TerminalId const& terminal_id) const;

	TerminalId root_terminal_id() const;
	std::vector<TerminalId> sink_terminal_ids() const;

	NodeId const& root_terminal() const;

	json export_to_json() const;

private:

private:
	Graph const _undirected_graph;
	Graph const _bidirected_graph;
	std::vector<NodeId> const _terminals;
};

} // namespace graph

#endif
