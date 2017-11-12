#ifndef TYPEDEFS_HPP
#define TYPEDEFS_HPP

#include <vector>
#include <deque>
#include <map>
#include <memory>
#include <limits>

#include "../debug.hpp"
#include "../Output.hpp"
#include "../helper/vector.hpp"

namespace graph {

using Id = std::size_t;

Id invalid_id();
bool is_id_valid(Id const& id, std::size_t const& num_ids);

using NodeId = Id;
using EdgeId = Id;
using Weight = double;
using NetId = Id;
using TerminalId = Id;

NodeId invalid_node_id();
EdgeId invalid_edge_id();
NetId invalid_net_id();
TerminalId invalid_terminal_id();

bool is_node_id_valid(NodeId const& node_id, std::size_t const& num_nodes);
bool is_edge_id_valid(EdgeId const& edge_id, std::size_t const& num_edges);
bool is_net_id_valid(NetId const& net_id, std::size_t const& num_nets);
bool is_terminal_id_valid(TerminalId const& terminal_id, std::size_t const& num_terminals);

} // namespace graph

#endif
