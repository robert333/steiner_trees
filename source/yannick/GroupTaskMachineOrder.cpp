#include "GroupTaskMachineOrder.hpp"

#include "../Logger.hpp"

namespace yannick {

GroupTaskMachineOrder::GroupTaskMachineOrder(
	YannickProblem const& yannick_problem,
	GroupTaskTime const& group_task_time,
	GroupTaskCycle const& group_task_cycle,
	GroupTaskMachine const& group_task_machine
) :
	Group("GroupTaskMachineOrder"),
	_yannick_problem(yannick_problem),
	_group_task_time(group_task_time),
	_group_task_cycle(group_task_cycle),
	_group_task_machine(group_task_machine)
{}

void GroupTaskMachineOrder::create_variables_constraints_and_objective(mip::MIPModel& mip_model)
{
	create_constraints(mip_model);
}

json GroupTaskMachineOrder::compute_solution() const
{
	json solution;

	for (graph::Node const& node_1 : _yannick_problem.precedence_graph().nodes()) {
		for (graph::Node const& node_2 : _yannick_problem.precedence_graph().nodes()) {
			if (node_1 == node_2) {
				continue;
			}

			for (Cycle cycle_1 = 0; cycle_1 < _yannick_problem.cycle_number(); ++cycle_1) {
				for (Cycle cycle_2 = 0; cycle_2 < _yannick_problem.cycle_number(); ++cycle_2) {
					for (Machine machine = 0; machine < _yannick_problem.machine_number(); ++machine) {
						solution["task_order"].push_back(
							{
								{"task_1", node_1.id()},
								{"task_2", node_2.id()},
								{"cycle_1", cycle_1},
								{"cycle_2", cycle_2},
								{"machine", machine}
							}
						);
					}
				}
			}
		}
	}

	return solution;
}

mip::VariableStorage<graph::NodeId, graph::NodeId, Cycle, Cycle, Machine> const& GroupTaskMachineOrder::variables() const
{
	return _variables;
}

void GroupTaskMachineOrder::create_constraints(mip::MIPModel& mip_model)
{
	for (graph::Node const& node_1 : _yannick_problem.precedence_graph().nodes()) {
		for (graph::Node const& node_2 : _yannick_problem.precedence_graph().nodes()) {
			if (node_1 == node_2) {
				continue;
			}

			for (Cycle cycle_1 = 0; cycle_1 < _yannick_problem.cycle_number(); ++cycle_1) {
				for (Cycle cycle_2 = 0; cycle_2 < _yannick_problem.cycle_number(); ++cycle_2) {
					for (Machine machine = 0; machine < _yannick_problem.machine_number(); ++machine) {
						create_order_constraint(mip_model, node_1, node_2, cycle_1, cycle_2, machine);
					}
				}
			}
		}
	}
}

void GroupTaskMachineOrder::create_order_constraint(
	mip::MIPModel& mip_model,
	graph::Node const& node_1,
	graph::Node const& node_2,
	Cycle cycle_1,
	Cycle cycle_2,
	Machine machine
)
{
	std::string const description = "If task " + node_1.to_string() + " in cycle " + std::to_string(cycle_1)
									+ " and task " + node_2.to_string() + " in cycle " + std::to_string(cycle_2)
									+ " are processed by machine " + std::to_string(machine)
									+ ", then they have to processed one after another!";

	Logger::logger() << "\n" << description << "\n";

	mip::Constraint constraint_1 = mip_model.create_constraint(description + "; decision 1");
	mip::Constraint constraint_2 = mip_model.create_constraint(description + "; decision 2");

	mip::MIPModel::Variable* const variable = mip_model.create_binary_variable(description + "; decision");
	_variables.set(node_1.id(), node_2.id(), cycle_1, cycle_2, machine, variable);

	constraint_1.add_variable(_group_task_time.variables().get(node_1.id()), 1);
	constraint_1.add_variable(_group_task_time.variables().get(node_2.id()), -1);
	constraint_1.set_lower_bound(
		(cycle_1 - cycle_2) * _yannick_problem.cycle_time() + node_1.weight()
	);
	constraint_1.lower_bound_condition_on(_group_task_cycle.variables().get(node_1.id(), cycle_1));
	constraint_1.lower_bound_condition_on(_group_task_cycle.variables().get(node_2.id(), cycle_2));
	constraint_1.lower_bound_condition_on(_group_task_machine.variables().get(node_1.id(), machine));
	constraint_1.lower_bound_condition_on(_group_task_machine.variables().get(node_2.id(), machine));
	constraint_1.lower_bound_condition_on(variables().get(node_1.id(), node_2.id(), cycle_1, cycle_2, machine), 0);

	constraint_2.add_variable(_group_task_time.variables().get(node_1.id()), -1);
	constraint_2.add_variable(_group_task_time.variables().get(node_2.id()), 1);
	constraint_2.set_lower_bound(
		(cycle_2 - cycle_1) * _yannick_problem.cycle_time() + node_2.weight()
	);
	constraint_2.lower_bound_condition_on(_group_task_cycle.variables().get(node_1.id(), cycle_1));
	constraint_2.lower_bound_condition_on(_group_task_cycle.variables().get(node_2.id(), cycle_2));
	constraint_2.lower_bound_condition_on(_group_task_machine.variables().get(node_1.id(), machine));
	constraint_2.lower_bound_condition_on(_group_task_machine.variables().get(node_2.id(), machine));
	constraint_2.lower_bound_condition_on(variables().get(node_1.id(), node_2.id(), cycle_1, cycle_2, machine), 1);
}

} // namespace yannick
