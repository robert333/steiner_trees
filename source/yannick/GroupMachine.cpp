#include "GroupMachine.hpp"

namespace yannick {

GroupMachine::GroupMachine(
	YannickProblem const& yannick_problem,
	GroupTaskMachine const& group_task_machine
) :
	Group("GroupMachine"),
	_yannick_problem(yannick_problem),
	_group_task_machine(group_task_machine)
{}

void GroupMachine::create_variables_constraints_and_objective(mip::MIPModel& mip_model)
{
	create_variables(mip_model);
	create_constraints(mip_model);
	create_objective(mip_model);
}

json GroupMachine::compute_solution() const
{
	json solution;

	for (Machine machine = 0; machine < _yannick_problem.machine_number(); ++machine) {
		solution["machine_value"].push_back(
			{
				{"machine", machine},
				{"value", variables().solution_value(machine)}
			}
		);

		if (variables().solution_value(machine) == 1) {
			solution["machines"].push_back(machine);
		}
	}

	return solution;
}

mip::VariableStorage<Machine> const& GroupMachine::variables() const
{
	return _variables;
}

void GroupMachine::create_variables(mip::MIPModel& mip_model)
{
	for (Machine machine = 0; machine < _yannick_problem.machine_number(); ++machine) {
		mip::MIPModel::Variable* const variable = mip_model.create_binary_variable(
			name(),
			"machine = " + std::to_string(machine)
		);
		_variables.set(machine, variable);
	}
}

void GroupMachine::create_constraints(mip::MIPModel& mip_model)
{
	for (Machine machine = 0; machine < _yannick_problem.machine_number(); ++machine) {
		for (graph::Node const& node : _yannick_problem.precedence_graph().nodes()) {
			mip::Constraint constraint = mip_model.create_constraint(
				name(),
				"machine " + std::to_string(machine) + " is used if task " + node.to_string() + " is assigned to it"
			);

			constraint.add_variable(variables().get(machine), 1);
			constraint.add_variable(_group_task_machine.variables().get(node.id(), machine), -1);

			constraint.set_lower_bound(0);
		}
	}

	create_constraint_machine_number_bounds(mip_model);
//	create_constraint_machines_that_have_to_be_used(mip_model);
}

void GroupMachine::create_objective(mip::MIPModel& mip_model)
{
	for (Machine machine = 0; machine < _yannick_problem.machine_number(); ++machine) {
		mip_model.set_objective_coefficient(variables().get(machine), 1);
	}
}

void GroupMachine::create_constraint_machine_number_bounds(mip::MIPModel& mip_model)
{
	mip::Constraint constraint = mip_model.create_constraint(
		name(),
		"we have to use at least " + std::to_string(_yannick_problem.min_machine_number())
		+ " and at most " + std::to_string(_yannick_problem.max_machine_number()) + " machines"
	);

	for (Machine machine = 0; machine < _yannick_problem.machine_number(); ++machine) {
		constraint.add_variable(variables().get(machine), 1);
	}

	constraint.set_lower_bound(_yannick_problem.min_machine_number());
	constraint.set_upper_bound(_yannick_problem.max_machine_number());
}

void GroupMachine::create_constraint_machines_that_have_to_be_used(mip::MIPModel& mip_model)
{
	for (Machine machine = 0; machine < _yannick_problem.min_machine_number(); ++machine) {
		mip::Constraint constraint = mip_model.create_constraint(
			name(),
			"we have to use machine" + std::to_string(machine)
		);

		constraint.add_variable(variables().get(machine), 1);

		constraint.set_lower_bound(1);
		constraint.set_upper_bound(1);
	}
}

} // namespace yannick
