#ifndef GRAPH_NET_HPP
#define GRAPH_NET_HPP

#include "typedefs.hpp"
#include "Node.hpp"
#include "../json.hpp"

namespace graph {

class Net : public Output {
public:
	using Vector = std::vector<Net>;
	using Name = std::string;

public:
	explicit Net(
		NetId const& id,
		std::string const& name,
		std::vector<NodeId> const& terminals,
		Weight const& weight = 1
	);

	NetId const& id() const;
	std::string const& name() const;
	std::vector<NodeId> const& terminals() const;
	Weight const& weight() const;

	std::size_t num_terminals() const;
	NodeId const& terminal(TerminalId const& terminal_id) const;

	bool is_terminal(NodeId const& node_id) const;

	std::string to_string() const final;

	json export_to_json() const;

private:

private:
	NetId _id;
	std::string _name;
	std::vector<NodeId> _terminals;
	Weight _weight;
};

} // namespace graph

#endif
