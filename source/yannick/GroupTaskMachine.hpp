#ifndef YANNICK_GROUPTASKMACHINE_HPP
#define YANNICK_GROUPTASKMACHINE_HPP

#include "../mip/Group.hpp"
#include "../mip/VariableStorage.hpp"
#include "../graph/Graph.hpp"
#include "../graph/Net.hpp"
#include "YannickProblem.hpp"
#include "GroupTaskTime.hpp"

namespace yannick {

class GroupTaskMachine : public mip::Group {
public:
	using SharedPtr = std::shared_ptr<GroupTaskMachine>;

public:
	explicit GroupTaskMachine(YannickProblem const& yannick_problem, GroupTaskTime const& group_task_time);

	void create_variables_constraints_and_objective(mip::MIPModel& mip_model) final;

	json compute_solution() const final;

	mip::VariableStorage<graph::NodeId, Machine> const& variables() const;

private:
	void create_variables(mip::MIPModel& mip_model);
	void create_constraints(mip::MIPModel& mip_model);
	void create_objective(mip::MIPModel& mip_model);

private:
	YannickProblem const& _yannick_problem;
	GroupTaskTime const& _group_task_time;

	mip::VariableStorage<graph::NodeId, Machine> _variables;
};

} // namespace yannick

#endif
