#ifndef YANNICK_YANNICKPROBLEM_HPP
#define YANNICK_YANNICKPROBLEM_HPP

#include "../graph/Graph.hpp"

namespace yannick {

using Index = int;
using Task = Index;
using Cycle = Index;
using Machine = Index;

class YannickProblem {
public:
	using SharedPtr = std::shared_ptr<YannickProblem>;
	using Vector = std::vector<YannickProblem>;

public:
	explicit YannickProblem(
		std::string const& name,
		graph::Graph const& precedence_graph,
		double cycle_time,
		Index min_cycle_number,
		Index max_cycle_number,
		Index min_machine_number,
		Index max_machine_number,
		Index min_jumper_number,
		Index max_jumper_number
	);

	std::string const& name() const;
	graph::Graph const& precedence_graph() const;
	double cycle_time() const;
	Index min_cycle_number() const;
	Index max_cycle_number() const;
	Index min_machine_number() const;
	Index max_machine_number() const;
	Index min_jumper_number() const;
	Index max_jumper_number() const;

	Index cycle_number() const;
	Index machine_number() const;

	bool is_it_possible_to_process_the_tasks_by_one_machine(graph::Node::Vector const& nodes) const;

	json export_to_json() const;

private:

private:
	std::string _name;
	graph::Graph _precedence_graph;
	double _cycle_time;
	Index _min_cycle_number;
	Index _max_cycle_number;
	Index _min_machine_number;
	Index _max_machine_number;
	Index _min_jumper_number;
	Index _max_jumper_number;
};

} // namespace yannick

#endif
