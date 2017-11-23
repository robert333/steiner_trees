#include "GroupTaskTime.hpp"
#include "../Logger.hpp"

namespace yannick {

GroupTaskTime::GroupTaskTime(YannickProblem const& yannick_problem) :
	Group("GroupTaskTime"),
	_yannick_problem(yannick_problem)
{}

void GroupTaskTime::create_variables_constraints_and_objective(mip::MIPModel& mip_model)
{
	create_variables(mip_model);
	create_constraints(mip_model);
}

json GroupTaskTime::compute_solution() const
{
	json solution;

	for (graph::Node const& node : _yannick_problem.precedence_graph().nodes()) {
		solution["tasks"].push_back({{"task", node.id()}, {"time", variables().solution_value(node.id())}});
	}

	return solution;
}

mip::VariableStorage<graph::NodeId> const& GroupTaskTime::variables() const
{
	return _variables;
}

void GroupTaskTime::create_variables(mip::MIPModel& mip_model)
{
	for (graph::Node const& node : _yannick_problem.precedence_graph().nodes()) {
		assert(node.weight() <= _yannick_problem.cycle_time());
		mip::MIPModel::Variable* const variable = mip_model.create_continuous_variable(
			name(), "node = " + node.to_string(), node.weight(), _yannick_problem.cycle_time()
		);
		_variables.set(node.id(), variable);
	}
}

void GroupTaskTime::create_constraints(mip::MIPModel& /*mip_model*/)
{
//	for (graph::Edge const& edge : _yannick_problem.precedence_graph().edges()) {
//		mip::Constraint constraint = mip_model.create_constraint(
//			"task " + std::to_string(edge.tail()) + " has to be processed before task " + std::to_string(edge.head())
//		);
//
//		constraint.add_variable(variables().get(edge.tail()), -1);
//		constraint.add_variable(variables().get(edge.head()), 1);
//
//		constraint.set_lower_bound(_yannick_problem.precedence_graph().node(edge.head()).weight());
//	}
}

} // namespace yannick
