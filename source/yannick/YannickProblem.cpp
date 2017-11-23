#include "YannickProblem.hpp"

namespace yannick {

YannickProblem::YannickProblem(
	std::string const& name,
	graph::Graph const& precedence_graph,
	double cycle_time,
	Index min_cycle_number,
	Index max_cycle_number,
	Index min_machine_number,
	Index max_machine_number,
	Index min_jumper_number,
	Index max_jumper_number
) :
	_name(name),
	_precedence_graph(precedence_graph),
	_cycle_time(cycle_time),
	_min_cycle_number(min_cycle_number),
	_max_cycle_number(max_cycle_number),
	_min_machine_number(min_machine_number),
	_max_machine_number(max_machine_number),
	_min_jumper_number(min_jumper_number),
	_max_jumper_number(max_jumper_number)
{
	assert(min_machine_number <= max_machine_number);
	assert(min_jumper_number <= max_jumper_number);
}

std::string const& YannickProblem::name() const
{
	return _name;
}

graph::Graph const& YannickProblem::precedence_graph() const
{
	return _precedence_graph;
}

double YannickProblem::cycle_time() const
{
	return _cycle_time;
}

Index YannickProblem::min_cycle_number() const
{
	return _min_cycle_number;
}

Index YannickProblem::max_cycle_number() const
{
	return _max_cycle_number;
}

Index YannickProblem::min_machine_number() const
{
	return _min_machine_number;
}

Index YannickProblem::max_machine_number() const
{
	return _max_machine_number;
}

Index YannickProblem::min_jumper_number() const
{
	return _min_jumper_number;
}

Index YannickProblem::max_jumper_number() const
{
	return _max_jumper_number;
}

Index YannickProblem::cycle_number() const
{
	return max_cycle_number();
}

Index YannickProblem::machine_number() const
{
	return max_machine_number();
}

bool YannickProblem::is_it_possible_to_process_the_tasks_by_one_machine(graph::Node::Vector const& nodes) const
{
	auto const sum_weight = [](graph::Weight weight, graph::Node const& node)->graph::Weight {
		return weight + node.weight();
	};

	return std::accumulate(nodes.begin(), nodes.end(), 0, sum_weight) <= cycle_time();
}

json YannickProblem::export_to_json() const
{
	return json();
}

} // namespace yannick
