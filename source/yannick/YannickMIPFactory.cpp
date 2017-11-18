#include "YannickMIPFactory.hpp"

#include "GroupTaskTime.hpp"
#include "GroupTaskCycle.hpp"
#include "GroupTaskMachine.hpp"
#include "GroupTaskMachineOrder.hpp"
#include "GroupTaskPrecedence.hpp"

namespace yannick {

mip::GroupManager YannickMIPFactory::create(YannickProblem const& yannick_problem)
{
	GroupTaskTime::SharedPtr group_task_time(
		std::make_shared<GroupTaskTime>(yannick_problem)
	);

	GroupTaskCycle::SharedPtr group_task_cycle(
		std::make_shared<GroupTaskCycle>(yannick_problem, *group_task_time)
	);

	GroupTaskMachine::SharedPtr group_task_machine(
		std::make_shared<GroupTaskMachine>(yannick_problem, *group_task_time)
	);

	GroupTaskMachineOrder::SharedPtr group_task_machine_order(
		std::make_shared<GroupTaskMachineOrder>(
			yannick_problem,
			*group_task_time,
			*group_task_cycle,
			*group_task_machine
		)
	);

	GroupTaskPrecedence::SharedPtr group_task_precedence(
		std::make_shared<GroupTaskPrecedence>(
			yannick_problem,
			*group_task_time,
			*group_task_cycle,
			*group_task_machine
		)
	);

	mip::GroupManager group_manager_steiner_tree_mip{};

	group_manager_steiner_tree_mip.add(group_task_time);
	group_manager_steiner_tree_mip.add(group_task_cycle);
	group_manager_steiner_tree_mip.add(group_task_machine);
//	group_manager_steiner_tree_mip.add(group_task_machine_order);
	group_manager_steiner_tree_mip.add(group_task_precedence);

	return group_manager_steiner_tree_mip;
}

} // namespace yannick
