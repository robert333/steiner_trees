// Edge.cpp
#include "Edge.hpp"

namespace graph {

Edge::Edge(
	EdgeId const& id,
	NodeId const& tail,
	NodeId const& head,
	Weight const& weight
) :
	_id(id),
	_tail(tail),
	_head(head),
	_weight(weight)
{}

EdgeId const& Edge::id() const
{
	return _id;
}

NodeId const& Edge::tail() const
{
	return _tail;
}

NodeId const& Edge::head() const
{
	return _head;
}

Weight const& Edge::weight() const
{
	return _weight;
}

NodeId const& Edge::opposite(NodeId const& node_id) const
{
	assert(node_id == tail() or node_id == head());
	return (node_id == tail()) ? head() : tail();
}

bool Edge::is_valid() const
{
	return tail() != invalid_node_id()
		   and head() != invalid_node_id()
		   and tail() != head();
}

bool Edge::operator==(Edge const& rhs) const
{
	return tail() == rhs.tail() and head() == rhs.head();
}

std::string Edge::to_string() const
{
	return "Edge[" + std::to_string(id()) + "] = {("
		   + std::to_string(tail()) + ", " + std::to_string(head())
		   + "), weight = " + std::to_string(weight()) + "}";
}

json Edge::export_to_json() const
{
	return {
		{"id", id()},
		{"tail", tail()},
		{"head", head()},
		{"weight", weight()}
	};
}

} // namespace graph
