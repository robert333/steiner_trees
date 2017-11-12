#include "Net.hpp"

namespace graph {

Net::Net(NetId const& id, std::string const& name, std::vector<NodeId> const& terminals, Weight const& weight) :
	_id(id),
	_name(name),
	_terminals(terminals),
	_weight(weight)
{}

NetId const& Net::id() const
{
	return _id;
}

std::string const& Net::name() const
{
	return _name;
}

std::vector<NodeId> const& Net::terminals() const
{
	return _terminals;
}

Weight const& Net::weight() const
{
	return _weight;
}

std::size_t Net::num_terminals() const
{
	return terminals().size();
}

NodeId const& Net::terminal(TerminalId const& terminal_id) const
{
	return terminals().at(terminal_id);
}

bool Net::is_terminal(NodeId const& node_id) const
{
	return helper::contains(terminals(), node_id);
}

std::string Net::to_string() const
{
	return "Net[" + std::to_string(id()) + "] = {name = " + name()
		   + ", " + helper::to_string(terminals(), "terminals")
		   + ", weight = " + std::to_string(weight()) + "}";
}

json Net::export_to_json() const
{
	return {
		{"id", id()},
		{"name", name()},
		{"terminals", terminals()}
	};
}

} // namespace graph
