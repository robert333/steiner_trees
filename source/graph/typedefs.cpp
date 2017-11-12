#include "typedefs.hpp"

namespace graph {

Id invalid_id()
{
	return std::numeric_limits<Id>::max();
}

bool is_id_valid(Id const& id, std::size_t const& num_ids)
{
	assert(id != invalid_id());
//	assert(id >= 0);
	assert(id < num_ids);
	return id != invalid_id() /*and id >= 0*/ and id < num_ids;
}

NodeId invalid_node_id()
{
	return invalid_id();
}

EdgeId invalid_edge_id()
{
	return invalid_id();
}

NetId invalid_net_id()
{
	return invalid_id();
}

TerminalId invalid_terminal_id()
{
	return invalid_id();
}

bool is_node_id_valid(NodeId const& node_id, std::size_t const& num_nodes)
{
	return is_id_valid(node_id, num_nodes);
}

bool is_edge_id_valid(EdgeId const& edge_id, std::size_t const& num_edges)
{
	return is_id_valid(edge_id, num_edges);
}

bool is_net_id_valid(NetId const& net_id, std::size_t const& num_nets)
{
	return is_id_valid(net_id, num_nets);
}

bool is_terminal_id_valid(TerminalId const& terminal_id, std::size_t const& num_terminals)
{
	return is_id_valid(terminal_id, num_terminals);
}

} // namespace graph
