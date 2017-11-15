#ifndef YANNICK_GROUPTASKMACHINEORDER_HPP
#define YANNICK_GROUPTASKMACHINEORDER_HPP

#include "../mip/Group.hpp"
#include "../mip/VariableStorage.hpp"
#include "../graph/Graph.hpp"
#include "../graph/Net.hpp"
#include "YannickProblem.hpp"
#include "GroupTaskTime.hpp"
#include "GroupTaskCycle.hpp"
#include "GroupTaskMachine.hpp"

namespace yannick {

class GroupTaskMachineOrder : public mip::Group {
public:
	using SharedPtr = std::shared_ptr<GroupTaskMachineOrder>;

public:
	explicit GroupTaskMachineOrder(
		YannickProblem const& yannick_problem,
		GroupTaskTime const& group_task_time,
		GroupTaskCycle const& group_task_cycle,
		GroupTaskMachine const& group_task_machine
	);

	void create_variables_constraints_and_objective(mip::MIPModel& mip_model) final;

	json compute_solution() const final;

	mip::VariableStorage<graph::NodeId, graph::NodeId, Cycle, Cycle, Machine> const& variables() const;

private:
	void create_constraints(mip::MIPModel& mip_model);

	void create_order_constraint(
		mip::MIPModel& mip_model,
		graph::Node const& node_1,
		graph::Node const& node_2,
		Cycle cycle_1,
		Cycle cycle_2,
		Machine machine
	);

private:
	YannickProblem const& _yannick_problem;
	GroupTaskTime const& _group_task_time;
	GroupTaskCycle const& _group_task_cycle;
	GroupTaskMachine const& _group_task_machine;

	mip::VariableStorage<graph::NodeId, graph::NodeId, Cycle, Cycle, Machine> _variables;
};

} // namespace yannick

#endif
