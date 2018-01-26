#include "GroupMachineCycle.hpp"
#include "../Logger.hpp"

namespace yannick {

GroupMachineCycle::GroupMachineCycle(
	YannickProblem const& yannick_problem,
	GroupMachine const& group_machine,
	GroupTaskMachine const& group_task_machine,
	GroupTaskCycle const& group_task_cycle
) :
	Group("GroupMachineCycle"),
	_yannick_problem(yannick_problem),
	_group_machine(group_machine),
	_group_task_machine(group_task_machine),
	_group_task_cycle(group_task_cycle)
{}

void GroupMachineCycle::create_variables_constraints_and_objective(mip::MIPModel& mip_model)
{
	create_variables(mip_model);
	create_constraints(mip_model);
}

json GroupMachineCycle::compute_solution() const
{
	json solution;

	for (Machine machine = 0; machine < _yannick_problem.machine_number(); ++machine) {
		json machine_solution;

		machine_solution["machine"] = machine;

		for (Cycle cycle = 0; cycle < _yannick_problem.cycle_number(); ++cycle) {
			machine_solution["cycles"].push_back(
				{
					{"cycle", cycle},
					{"value", variables().solution_value(machine, cycle)}
				}
			);
		}

		solution["solution"].push_back(machine_solution);
	}

	return solution;
}

mip::VariableStorage<Machine, Cycle> const& GroupMachineCycle::variables() const
{
	return _variables;
}

void GroupMachineCycle::create_variables(mip::MIPModel& mip_model)
{
	for (Machine machine = 0; machine < _yannick_problem.machine_number(); ++machine) {
		for (Cycle cycle = 0; cycle < _yannick_problem.cycle_number(); ++cycle) {
			mip::MIPModel::Variable* const variable = mip_model.create_binary_variable(
				name(), "machine = " + std::to_string(machine) + ", cycle = " + std::to_string(cycle)
			);
			_variables.set(machine, cycle, variable);
		}
	}
}

void GroupMachineCycle::create_constraints(mip::MIPModel& mip_model)
{
	for (Machine machine = 0; machine < _yannick_problem.machine_number(); ++machine) {
		mip::Constraint constraint_no_jumps = mip_model.create_constraint(
			name(), "machine = " + std::to_string(machine)
		);

		constraint_no_jumps.set_upper_bound(1);

		for (Cycle cycle = 0; cycle < _yannick_problem.cycle_number(); ++cycle) {
			constraint_no_jumps.add_variable(variables().get(machine, cycle), 1);

			for (graph::Node const& node : _yannick_problem.precedence_graph().nodes()) {
				mip::Constraint constraint = mip_model.create_constraint(
					name(),
					"machine = " + std::to_string(machine)
					+ ", cycle = " + std::to_string(cycle)
					+ ", task = " + node.to_string()
				);

				constraint.set_lower_bound(0);

				constraint.add_variable(_group_machine.variables().get(machine), 1);
				constraint.add_variable(variables().get(machine, cycle), 1);

				constraint.add_variable(_group_task_machine.variables().get(node.id(), machine), -1);
				constraint.add_variable(_group_task_cycle.variables().get(node.id(), cycle), -1);
			}
		}
	}
}

} // namespace yannick
