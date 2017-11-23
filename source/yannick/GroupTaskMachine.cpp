#include "GroupTaskMachine.hpp"
#include "../Logger.hpp"

namespace yannick {

GroupTaskMachine::GroupTaskMachine(YannickProblem const& yannick_problem, GroupTaskTime const& group_task_time) :
	Group("GroupTaskMachine"),
	_yannick_problem(yannick_problem),
	_group_task_time(group_task_time)
{}

void GroupTaskMachine::create_variables_constraints_and_objective(mip::MIPModel& mip_model)
{
	create_variables(mip_model);
	create_constraints(mip_model);
	create_objective(mip_model);
}

json GroupTaskMachine::compute_solution() const
{
	json solution;

	for (graph::Node const& node : _yannick_problem.precedence_graph().nodes()) {
		json task_solution;

		task_solution["task"] = node.id();

		for (Machine machine = 0; machine < _yannick_problem.machine_number(); ++machine) {
			task_solution["machines"].push_back(
				{
					{"machine", machine},
					{"value", variables().solution_value(node.id(), machine)}
				}
			);
		}

		solution["solution"].push_back(task_solution);
	}

	for (Machine machine = 0; machine < _yannick_problem.machine_number(); ++machine) {
		std::vector<Task> assigned_tasks;

		for (graph::Node const& node : _yannick_problem.precedence_graph().nodes()) {
			if (variables().solution_value(node.id(), machine) == 1) {
				assigned_tasks.push_back(static_cast<Task>(node.id()));
			}
		}

		auto const sum_time = [&](double time, Task const& task)->graph::Weight {
			return time + _yannick_problem.precedence_graph().node(static_cast<graph::NodeId>(task)).weight();
		};

		assert(
			std::accumulate(
				assigned_tasks.begin(), assigned_tasks.end(), 0, sum_time
			) <= _yannick_problem.cycle_time()
		);
		DUMMY_USE(sum_time);

		solution["machines"].push_back({{"machine", machine}, {"tasks", assigned_tasks}});
	}

	return solution;
}

mip::VariableStorage<graph::NodeId, Machine> const& GroupTaskMachine::variables() const
{
	return _variables;
}

void GroupTaskMachine::create_variables(mip::MIPModel& mip_model)
{
	for (graph::Node const& node : _yannick_problem.precedence_graph().nodes()) {
		for (Machine machine = 0; machine < _yannick_problem.machine_number(); ++machine) {
			mip::MIPModel::Variable* const variable = mip_model.create_binary_variable(
				name(), "task = " + node.to_string() + ", machine = " + std::to_string(machine)
			);
			_variables.set(node.id(), machine, variable);
		}
	}
}

void GroupTaskMachine::create_constraints(mip::MIPModel& mip_model)
{
	for (graph::Node const& node : _yannick_problem.precedence_graph().nodes()) {
		mip::Constraint constraint = mip_model.create_constraint(
			name(), "exact one machine per task : task = " + node.to_string()
		);

		for (Machine machine = 0; machine < _yannick_problem.machine_number(); ++machine) {
			constraint.add_variable(variables().get(node.id(), machine), 1);
		}

		constraint.set_lower_bound(1);
		constraint.set_upper_bound(1);
	}

	for (Machine machine = 0; machine < _yannick_problem.machine_number(); ++machine) {
		mip::Constraint constraint = mip_model.create_constraint(
			name(), "tasks processed by machine " + std::to_string(machine) + " need to fit into one cycle time"
		);

		for (graph::Node const& node : _yannick_problem.precedence_graph().nodes()) {
			constraint.add_variable(variables().get(node.id(), machine), node.weight());
		}

		constraint.set_upper_bound(_yannick_problem.cycle_time());
	}

//	for (graph::Node const& node_1 : _yannick_problem.precedence_graph().nodes()) {
//		for (graph::Node const& node_2 : _yannick_problem.precedence_graph().nodes()) {
//			if (node_1 >= node_2) {
//				continue;
//			}
//
//			if (not _yannick_problem.is_it_possible_to_process_the_tasks_by_one_machine({node_1, node_2})) {
//				for (Machine machine = 0; machine < _yannick_problem.machine_number(); ++machine) {
//					mip::Constraint constraint = mip_model.create_constraint(
//						"task " + node_1.to_string() + " and task " + node_2.to_string()
//						+ " processed by machine " + std::to_string(machine)
//						+ " is not possible"
//					);
//
//					Logger::logger() << constraint.name() << "\n";
//
//					constraint.add_variable(variables().get(node_1.id(), machine), 1);
//					constraint.add_variable(variables().get(node_2.id(), machine), 1);
//
//					constraint.set_upper_bound(1);
//				}
//			}
//		}
//	}
}

void GroupTaskMachine::create_objective(mip::MIPModel& /*mip_model*/)
{
//	for (graph::Node const& node : _yannick_problem.precedence_graph().nodes()) {
//		for (Machine machine = 0; machine < _yannick_problem.machine_number(); ++machine) {
//			mip_model.set_objective_coefficient(_variables.get(node.id(), machine), 1 + machine);
//		}
//	}
}

} // namespace yannick
