#include "TerminalInstance.hpp"

namespace graph {

TerminalInstance::TerminalInstance(
	Graph const& undirected_graph,
	std::vector<NodeId> const& terminals
) :
	_undirected_graph(undirected_graph),
	_bidirected_graph(undirected_graph.bidirect()),
	_terminals(terminals)
{
	assert(not undirected_graph.is_directed());
	assert(not terminals.empty());
}

Graph const& TerminalInstance::undirected_graph() const
{
	return _undirected_graph;
}

Graph const& TerminalInstance::bidirected_graph() const
{
	return _bidirected_graph;
}

std::vector<NodeId> const& TerminalInstance::terminals() const
{
	return _terminals;
}

NodeId const& TerminalInstance::terminal(TerminalId const& terminal_id) const
{
	return terminals().at(terminal_id);
}

TerminalId TerminalInstance::root_terminal_id() const
{
	return 0;
}

std::vector<TerminalId> TerminalInstance::sink_terminal_ids() const
{
	std::vector<graph::NodeId> result;

	for (graph::TerminalId terminal_id = 0; terminal_id < terminals().size(); ++terminal_id) {
		if (terminal_id != root_terminal_id()) {
			result.push_back(terminal_id);
		}
	}

	return result;
}

NodeId const& TerminalInstance::root_terminal() const
{
	return terminal(root_terminal_id());
}

json TerminalInstance::export_to_json() const
{
	return {
		{"graph", undirected_graph().export_to_json()},
		{"terminals", terminals()}
	};
}

} // namespace graph
