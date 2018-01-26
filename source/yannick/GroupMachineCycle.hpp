#ifndef YANNICK_GROUPMACHINECYCLE_HPP
#define YANNICK_GROUPMACHINECYCLE_HPP

#include "../mip/Group.hpp"
#include "../mip/VariableStorage.hpp"
#include "../graph/Graph.hpp"
#include "../graph/Net.hpp"
#include "YannickProblem.hpp"
#include "GroupMachine.hpp"
#include "GroupTaskCycle.hpp"

namespace yannick {

class GroupMachineCycle : public mip::Group {
public:
	using SharedPtr = std::shared_ptr<GroupMachineCycle>;

public:
	explicit GroupMachineCycle(
		YannickProblem const& yannick_problem,
		GroupMachine const& group_machine,
		GroupTaskMachine const& group_task_machine,
		GroupTaskCycle const& group_task_cycle
	);

	void create_variables_constraints_and_objective(mip::MIPModel& mip_model) final;

	json compute_solution() const final;

	mip::VariableStorage<Machine, Cycle> const& variables() const;

private:
	void create_variables(mip::MIPModel& mip_model);
	void create_constraints(mip::MIPModel& mip_model);

private:
	YannickProblem const& _yannick_problem;
	GroupMachine const& _group_machine;
	GroupTaskMachine const& _group_task_machine;
	GroupTaskCycle const& _group_task_cycle;

	mip::VariableStorage<Machine, Cycle> _variables;
};

} // namespace yannick

#endif
