#include "GroupTaskMachineOrder.hpp"

#include "../Logger.hpp"
#include "../graph/algorithms/Dijkstra.hpp"
#include "../graph/algorithms/MooreBellmanFord.hpp"

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
			if (node_1 >= node_2) {
				continue;
			}

			if (not _yannick_problem.is_it_possible_to_process_the_tasks_by_one_machine({node_1, node_2})) {
				continue;
			}

			for (Cycle cycle_1 = 0; cycle_1 < _yannick_problem.cycle_number(); ++cycle_1) {
				for (Cycle cycle_2 = 0; cycle_2 < _yannick_problem.cycle_number(); ++cycle_2) {
					create_order_constraints(mip_model, node_1, node_2, cycle_1, cycle_2);
				}
			}
		}
	}
}

void GroupTaskMachineOrder::create_order_constraints(
	mip::MIPModel& mip_model,
	graph::Node const& node_1,
	graph::Node const& node_2,
	Cycle cycle_1,
	Cycle cycle_2
)
{
	bool task_1_before_task_2_is_not_possible = false;
	bool task_2_before_task_1_is_not_possible = false;

	graph::Weight const longest_path_from_node_1_to_node_2 = graph::MooreBellmanFord::compute_longest_path(
		_yannick_problem.precedence_graph(), node_1, node_2
	);

	graph::Weight const longest_path_from_node_2_to_node_1 = graph::MooreBellmanFord::compute_longest_path(
		_yannick_problem.precedence_graph(), node_2, node_1
	);

	Logger::logger() << "longest path from node " << node_1.to_string() << " to node " << node_2.to_string() << ": "
					 << longest_path_from_node_1_to_node_2 << "\n";
	Logger::logger() << "longest path from node " << node_2.to_string() << " to node " << node_1.to_string() << ": "
					 << longest_path_from_node_2_to_node_1 << "\n";

	if (node_1.weight() + longest_path_from_node_2_to_node_1 > (cycle_1 - cycle_2) * _yannick_problem.cycle_time()) {
		task_1_before_task_2_is_not_possible = true;
		Logger::logger() << "Processing task " << node_1.to_string() << " in cycle " << std::to_string(cycle_1)
						 << " before task " + node_2.to_string() << " in cycle " + std::to_string(cycle_2)
						 << " is not possible!\n";
	}

	if (node_2.weight() + longest_path_from_node_1_to_node_2 > (cycle_2 - cycle_1) * _yannick_problem.cycle_time()) {
		task_2_before_task_1_is_not_possible = true;
		Logger::logger() << "Processing task " << node_2.to_string() << " in cycle " << std::to_string(cycle_2)
						 << " before task " + node_1.to_string() << " in cycle " + std::to_string(cycle_1)
						 << " is not possible!\n";
	}

	for (Machine machine = 0; machine < _yannick_problem.machine_number(); ++machine) {
		if (task_1_before_task_2_is_not_possible and task_2_before_task_1_is_not_possible) {
			mip::Constraint constraint = mip_model.create_constraint(
				name(),
				"task " + node_1.to_string() + " in cycle " + std::to_string(cycle_1)
				+ " and task " + node_2.to_string() + " in cycle " + std::to_string(cycle_2)
				+ " processed by machine " + std::to_string(machine)
				+ " is not possible"
			);

			Logger::logger() << constraint.name() << "\n";

			constraint.add_variable(_group_task_cycle.variables().get(node_1.id(), cycle_1), 1);
			constraint.add_variable(_group_task_cycle.variables().get(node_2.id(), cycle_2), 1);
			constraint.add_variable(_group_task_machine.variables().get(node_1.id(), machine), 1);
			constraint.add_variable(_group_task_machine.variables().get(node_2.id(), machine), 1);

			constraint.set_upper_bound(3);
		}

		mip::Constraint constraint_1;
		mip::Constraint constraint_2;

		if (not task_1_before_task_2_is_not_possible) {
			constraint_1 = create_order_constraint(mip_model, node_1, cycle_1, node_2, cycle_2, machine);
		}

		if (not task_2_before_task_1_is_not_possible) {
			constraint_2 = create_order_constraint(mip_model, node_2, cycle_2, node_1, cycle_1, machine);
		}

		if (not task_1_before_task_2_is_not_possible and not task_2_before_task_1_is_not_possible) {
			mip::MIPModel::Variable* const variable = mip_model.create_binary_variable(
				name(),
				"decision for the order of task " + node_1.to_string() + " in cycle " + std::to_string(cycle_1)
				+ " and task " + node_2.to_string() + " in cycle " + std::to_string(cycle_2)
				+ " processed by machine " + std::to_string(machine)
			);

			_variables.set(node_1.id(), node_2.id(), cycle_1, cycle_2, machine, variable);

			constraint_1.lower_bound_condition_on(
				variables().get(node_1.id(), node_2.id(), cycle_1, cycle_2, machine), 0
			);
			constraint_2.lower_bound_condition_on(
				variables().get(node_1.id(), node_2.id(), cycle_1, cycle_2, machine), 1
			);
		}
	}

	Logger::logger() << "\n";
}

mip::Constraint GroupTaskMachineOrder::create_order_constraint(
	mip::MIPModel& mip_model,
	graph::Node const& node_first,
	Cycle cycle_first,
	graph::Node const& node_second,
	Cycle cycle_second,
	Machine machine
)
{
	mip::Constraint constraint = mip_model.create_constraint(
		name(),
		"task " + node_first.to_string()
		+ " in cycle " + std::to_string(cycle_first)
		+ " will be processed before task " + node_second.to_string()
		+ " in cycle " + std::to_string(cycle_second)
		+ " by machine " + std::to_string(machine)
	);

	constraint.add_variable(_group_task_time.variables().get(node_first.id()), -1);
	constraint.add_variable(_group_task_time.variables().get(node_second.id()), 1);

	constraint.set_lower_bound((cycle_second - cycle_first) * _yannick_problem.cycle_time() + node_second.weight());

	constraint.lower_bound_condition_on(_group_task_cycle.variables().get(node_first.id(), cycle_first));
	constraint.lower_bound_condition_on(_group_task_cycle.variables().get(node_second.id(), cycle_second));
	constraint.lower_bound_condition_on(_group_task_machine.variables().get(node_first.id(), machine));
	constraint.lower_bound_condition_on(_group_task_machine.variables().get(node_second.id(), machine));

	return constraint;
}

} // namespace yannick
