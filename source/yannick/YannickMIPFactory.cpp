#include "YannickMIPFactory.hpp"

#include "GroupTaskTime.hpp"
#include "GroupTaskCycle.hpp"
#include "GroupTaskMachine.hpp"
#include "GroupTaskMachineOrder.hpp"
#include "GroupTaskPrecedence.hpp"
#include "GroupMachine.hpp"
#include "GroupCycle.hpp"
#include "GroupMachineCycle.hpp"

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

	GroupMachine::SharedPtr group_machine(
		std::make_shared<GroupMachine>(
			yannick_problem,
			*group_task_machine
		)
	);

	GroupCycle::SharedPtr group_cycle(
		std::make_shared<GroupCycle>(
			yannick_problem,
			*group_task_cycle
		)
	);

	GroupMachineCycle::SharedPtr group_machine_cycle(
		std::make_shared<GroupMachineCycle>(
			yannick_problem,
			*group_machine,
			*group_task_machine,
			*group_task_cycle
		)
	);

	mip::GroupManager group_manager_steiner_tree_mip{};

	group_manager_steiner_tree_mip.add(group_task_time);
	group_manager_steiner_tree_mip.add(group_task_cycle);
	group_manager_steiner_tree_mip.add(group_task_machine);
//	group_manager_steiner_tree_mip.add(group_task_machine_order);
	group_manager_steiner_tree_mip.add(group_task_precedence);
	group_manager_steiner_tree_mip.add(group_machine);
//	group_manager_steiner_tree_mip.add(group_cycle);
	group_manager_steiner_tree_mip.add(group_machine_cycle);

	return group_manager_steiner_tree_mip;
}

} // namespace yannick
