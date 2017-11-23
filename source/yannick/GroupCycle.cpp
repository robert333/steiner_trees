#include "GroupCycle.hpp"

namespace yannick {

GroupCycle::GroupCycle(
	YannickProblem const& yannick_problem,
	GroupTaskCycle const& group_task_cycle
) :
	Group("GroupCycle"),
	_yannick_problem(yannick_problem),
	_group_task_cycle(group_task_cycle)
{}

void GroupCycle::create_variables_constraints_and_objective(mip::MIPModel& mip_model)
{
	create_variables(mip_model);
	create_constraints(mip_model);
	create_objective(mip_model);
}

json GroupCycle::compute_solution() const
{
	json solution;

	for (Cycle cycle = 0; cycle < _yannick_problem.cycle_number(); ++cycle) {
		solution["cycle_value"].push_back(
			{
				{"cycle", cycle},
				{"value", variables().solution_value(cycle)}
			}
		);

		if (variables().solution_value(cycle) == 1) {
			solution["cycles"].push_back(cycle);
		}
	}

	return solution;
}

mip::VariableStorage<Cycle> const& GroupCycle::variables() const
{
	return _variables;
}

void GroupCycle::create_variables(mip::MIPModel& mip_model)
{
	for (Cycle cycle = 0; cycle < _yannick_problem.cycle_number(); ++cycle) {
		mip::MIPModel::Variable* const variable = mip_model.create_binary_variable(
			name(), "cycle = " + std::to_string(cycle)
		);
		_variables.set(cycle, variable);
	}
}

void GroupCycle::create_constraints(mip::MIPModel& mip_model)
{
	for (Cycle cycle = 0; cycle < _yannick_problem.cycle_number(); ++cycle) {
		for (graph::Node const& node : _yannick_problem.precedence_graph().nodes()) {
			mip::Constraint constraint = mip_model.create_constraint(
				name(),
				"cycle " + std::to_string(cycle) + " is needed if task " + node.to_string() + " is assigned to it"
			);

			constraint.add_variable(variables().get(cycle), 1);
			constraint.add_variable(_group_task_cycle.variables().get(node.id(), cycle), -1);

			constraint.set_lower_bound(0);
		}
	}

	mip::Constraint constraint = mip_model.create_constraint(
		name(),
		"We have to use at least " + std::to_string(_yannick_problem.min_cycle_number())
		+ " and at most " + std::to_string(_yannick_problem.max_cycle_number()) + " cycles"
	);

	for (Cycle cycle = 0; cycle < _yannick_problem.cycle_number(); ++cycle) {
		constraint.add_variable(variables().get(cycle), 1);
	}

	constraint.set_lower_bound(_yannick_problem.min_cycle_number());
	constraint.set_upper_bound(_yannick_problem.max_cycle_number());
}

void GroupCycle::create_objective(mip::MIPModel& /*mip_model*/)
{
//	for (Cycle cycle = 0; cycle < _yannick_problem.cycle_number(); ++cycle) {
//		mip_model.set_objective_coefficient(variables().get(cycle), 1);
//	}
}

} // namespace yannick
