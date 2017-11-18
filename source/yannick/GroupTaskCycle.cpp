#include "GroupTaskCycle.hpp"
#include "../Logger.hpp"

namespace yannick {

GroupTaskCycle::GroupTaskCycle(
	YannickProblem const& yannick_problem,
	GroupTaskTime const& group_task_time
) :
	Group("GroupTaskCycle"),
	_yannick_problem(yannick_problem),
	_group_task_time(group_task_time)
{}

void GroupTaskCycle::create_variables_constraints_and_objective(mip::MIPModel& mip_model)
{
	create_variables(mip_model);
	create_constraints(mip_model);
}

json GroupTaskCycle::compute_solution() const
{
	json solution;

	for (graph::Node const& node : _yannick_problem.precedence_graph().nodes()) {
		for (Cycle cycle = 0; cycle < _yannick_problem.cycle_number(); ++cycle) {
			if (variables().solution_value(node.id(), cycle) == 1) {
				solution["task_to_cycle"].push_back({{"task", node.id()}, {"cycle", cycle}});
			}
		}
	}

	for (Cycle cycle = 0; cycle < _yannick_problem.cycle_number(); ++cycle) {
		std::vector<Task> assigned_tasks;

		for (graph::Node const& node : _yannick_problem.precedence_graph().nodes()) {
			if (variables().solution_value(node.id(), cycle) == 1) {
				assigned_tasks.push_back(static_cast<Task>(node.id()));
			}
		}

		solution["cycles"].push_back({{"cycle", cycle}, {"tasks", assigned_tasks}});
	}

	return solution;
}

mip::VariableStorage<graph::NodeId, Cycle> const& GroupTaskCycle::variables() const
{
	return _variables;
}

void GroupTaskCycle::create_variables(mip::MIPModel& mip_model)
{
	for (graph::Node const& node : _yannick_problem.precedence_graph().nodes()) {
		for (Cycle cycle = 0; cycle < _yannick_problem.cycle_number(); ++cycle) {
			mip::MIPModel::Variable* const variable = mip_model.create_binary_variable(
				"task = " + node.to_string() + ", cycle = " + std::to_string(cycle)
			);
			_variables.set(node.id(), cycle, variable);
		}
	}
}

void GroupTaskCycle::create_constraints(mip::MIPModel& mip_model)
{
	for (graph::Node const& node : _yannick_problem.precedence_graph().nodes()) {
		mip::Constraint constraint = mip_model.create_constraint(
			"exact one cycle per task " + node.to_string()
		);

		for (Cycle cycle = 0; cycle < _yannick_problem.cycle_number(); ++cycle) {
			constraint.add_variable(variables().get(node.id(), cycle), 1);
		}

		constraint.set_lower_bound(1);
		constraint.set_upper_bound(1);
	}

//	for (graph::Node const& node : _yannick_problem.precedence_graph().nodes()) {
//		for (Cycle cycle = 0; cycle < _yannick_problem.cycle_number(); ++cycle) {
//			mip::Constraint constraint_lower = mip_model.create_constraint(
//				"if task " + node.to_string() + " in cycle " + std::to_string(cycle)
//				+ " then set lower time slot bound"
//			);
//			mip::Constraint constraint_upper = mip_model.create_constraint(
//				"if task " + node.to_string() + " in cycle " + std::to_string(cycle)
//				+ " then set upper time slot bound"
//			);
//
//			constraint_lower.add_variable(_group_task_time.variables().get(node.id()), 1);
//			constraint_lower.set_lower_bound(cycle * _yannick_problem.cycle_time() + node.weight());
//			constraint_lower.lower_bound_condition_on(variables().get(node.id(), cycle));
//
//			constraint_upper.add_variable(_group_task_time.variables().get(node.id()), 1);
//			constraint_upper.set_upper_bound((cycle + 1) * _yannick_problem.cycle_time());
//			constraint_upper.upper_bound_condition_on(variables().get(node.id(), cycle));
//		}
//	}
}

} // namespace yannick
