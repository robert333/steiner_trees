#ifndef YANNICK_GROUPMACHINE_HPP
#define YANNICK_GROUPMACHINE_HPP

#include "../mip/Group.hpp"
#include "../mip/VariableStorage.hpp"
#include "../graph/Graph.hpp"
#include "../graph/Net.hpp"
#include "YannickProblem.hpp"
#include "GroupTaskMachine.hpp"

namespace yannick {

class GroupMachine : public mip::Group {
public:
	using SharedPtr = std::shared_ptr<GroupMachine>;

public:
	explicit GroupMachine(
		YannickProblem const& yannick_problem,
		GroupTaskMachine const& group_task_machine
	);

	void create_variables_constraints_and_objective(mip::MIPModel& mip_model) final;

	json compute_solution() const final;

	mip::VariableStorage<Machine> const& variables() const;

private:
	void create_variables(mip::MIPModel& mip_model);
	void create_constraints(mip::MIPModel& mip_model);
	void create_objective(mip::MIPModel& mip_model);

	void create_constraint_machine_number_bounds(mip::MIPModel& mip_model);
	void create_constraint_machines_that_have_to_be_used(mip::MIPModel& mip_model);

private:
	YannickProblem const& _yannick_problem;
	GroupTaskMachine const& _group_task_machine;

	mip::VariableStorage<Machine> _variables;
};

} // namespace yannick

#endif
