#include "GroupTaskPrecedence.hpp"
#include "../Logger.hpp"

namespace yannick {

GroupTaskPrecedence::GroupTaskPrecedence(
	YannickProblem const& yannick_problem,
	GroupTaskTime const& group_task_time,
	GroupTaskCycle const& group_task_cycle,
	GroupTaskMachine const& group_task_machine
) :
	Group("GroupTaskPrecedence"),
	_yannick_problem(yannick_problem),
	_group_task_time(group_task_time),
	_group_task_cycle(group_task_cycle),
	_group_task_machine(group_task_machine)
{}

void GroupTaskPrecedence::create_variables_constraints_and_objective(mip::MIPModel& mip_model)
{
	create_constraints(mip_model);
}

void GroupTaskPrecedence::create_constraints(mip::MIPModel& mip_model)
{
	for (graph::Edge const& edge : _yannick_problem.precedence_graph().edges()) {
		for (Cycle cycle = 0; cycle < _yannick_problem.cycle_number(); ++cycle) {
			mip::Constraint constraint = mip_model.create_constraint(
				"if task " + std::to_string(edge.head()) + " is processed in cycle " + std::to_string(cycle)
				+ ", then task " + std::to_string(edge.tail())
				+ " cannot be processed in upcoming cycles by the precedence graph"
			);

			for (Cycle upcoming_cycle = cycle + 1; upcoming_cycle < _yannick_problem.cycle_number(); ++upcoming_cycle) {
				constraint.add_variable(_group_task_cycle.variables().get(edge.tail(), upcoming_cycle), 1);
			}

			constraint.set_upper_bound(0);

			constraint.upper_bound_condition_on(_group_task_cycle.variables().get(edge.head(), cycle));
		}

		for (Cycle cycle = 0; cycle < _yannick_problem.cycle_number(); ++cycle) {
			mip::Constraint constraint = mip_model.create_constraint(
				"if task " + std::to_string(edge.tail()) + " and task " + std::to_string(edge.head())
				+ " are assigned to cycle " + std::to_string(cycle)
				+ ", then task " + std::to_string(edge.tail())
				+ " have to be processed before task " + std::to_string(edge.head())
				+ " by the precedence graph"
			);

			constraint.add_variable(_group_task_time.variables().get(edge.tail()), -1);
			constraint.add_variable(_group_task_time.variables().get(edge.head()), 1);

			constraint.set_lower_bound(_yannick_problem.precedence_graph().node(edge.head()).weight());

			constraint.lower_bound_condition_on(_group_task_cycle.variables().get(edge.tail(), cycle));
			constraint.lower_bound_condition_on(_group_task_cycle.variables().get(edge.head(), cycle));
		}
	}

	for (graph::Node const& node_1 : _yannick_problem.precedence_graph().nodes()) {
		for (graph::Node const& node_2 : _yannick_problem.precedence_graph().nodes()) {
			if (node_1 >= node_2) {
				continue;
			}

			bool task_1_before_task_2_is_not_possible = false;
			bool task_2_before_task_1_is_not_possible = false;

			for (Machine machine = 0; machine < _yannick_problem.machine_number(); ++machine) {
				mip::Constraint constraint_1;
				mip::Constraint constraint_2;

				if (not task_1_before_task_2_is_not_possible) {
					constraint_1 = create_order_constraint(mip_model, node_1, node_2, machine);
				}

				if (not task_2_before_task_1_is_not_possible) {
					constraint_2 = create_order_constraint(mip_model, node_2, node_1, machine);
				}

				if (not task_1_before_task_2_is_not_possible and not task_2_before_task_1_is_not_possible) {
					mip::MIPModel::Variable* const variable = mip_model.create_binary_variable(
						"decision for the order of task " + node_1.to_string()
						+ " and task " + node_2.to_string()
						+ " processed by machine " + std::to_string(machine)
					);

					constraint_1.lower_bound_condition_on(variable, 0);
					constraint_2.lower_bound_condition_on(variable, 1);
				}
			}
		}
	}
}

mip::Constraint GroupTaskPrecedence::create_order_constraint(
	mip::MIPModel& mip_model,
	graph::Node const& node_first,
	graph::Node const& node_second,
	Machine machine
)
{
	mip::Constraint constraint = mip_model.create_constraint(
		"task " + node_first.to_string()
		+ " will be processed before task " + node_second.to_string()
		+ " by machine " + std::to_string(machine)
	);

	constraint.add_variable(_group_task_time.variables().get(node_first.id()), -1);
	constraint.add_variable(_group_task_time.variables().get(node_second.id()), 1);

	constraint.set_lower_bound(node_second.weight());

	constraint.lower_bound_condition_on(_group_task_machine.variables().get(node_first.id(), machine));
	constraint.lower_bound_condition_on(_group_task_machine.variables().get(node_second.id(), machine));

	return constraint;
}

} // namespace yannick
