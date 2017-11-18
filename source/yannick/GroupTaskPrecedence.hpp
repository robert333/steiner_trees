#ifndef YANNICK_GROUPTASKPRECEDENCE_HPP
#define YANNICK_GROUPTASKPRECEDENCE_HPP

#include "GroupTaskTime.hpp"
#include "GroupTaskCycle.hpp"
#include "GroupTaskMachine.hpp"

namespace yannick {

class GroupTaskPrecedence : public mip::Group {
public:
	using SharedPtr = std::shared_ptr<GroupTaskPrecedence>;

public:
	explicit GroupTaskPrecedence(
		YannickProblem const& yannick_problem,
		GroupTaskTime const& group_task_time,
		GroupTaskCycle const& group_task_cycle,
		GroupTaskMachine const& group_task_machine
	);

	void create_variables_constraints_and_objective(mip::MIPModel& mip_model) final;

private:
	void create_constraints(mip::MIPModel& mip_model);

	mip::Constraint create_order_constraint(
		mip::MIPModel& mip_model,
		graph::Node const& node_first,
		graph::Node const& node_second,
		Machine machine
	);

private:
	YannickProblem const& _yannick_problem;
	GroupTaskTime const& _group_task_time;
	GroupTaskCycle const& _group_task_cycle;
	GroupTaskMachine const& _group_task_machine;
};

} // namespace yannick

#endif
