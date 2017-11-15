#ifndef YANNICK_YANNICKPROBLEM_HPP
#define YANNICK_YANNICKPROBLEM_HPP

#include "../graph/Graph.hpp"

namespace yannick {

using Index = int;
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
		Index cycle_number,
		Index machine_number
	);

	std::string const& name() const;
	graph::Graph const& precedence_graph() const;
	double cycle_time() const;
	Index cycle_number() const;
	Index machine_number() const;

	json export_to_json() const;

private:

private:
	std::string _name;
	graph::Graph _precedence_graph;
	double _cycle_time;
	Index _cycle_number;
	Index _machine_number;
};

} // namespace yannick

#endif
