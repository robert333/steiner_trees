#ifndef YANNICK_GROUPTASKCYCLE_HPP
#define YANNICK_GROUPTASKCYCLE_HPP

#include "../mip/Group.hpp"
#include "../mip/VariableStorage.hpp"
#include "../graph/Graph.hpp"
#include "../graph/Net.hpp"
#include "YannickProblem.hpp"
#include "GroupTaskTime.hpp"

namespace yannick {

class GroupTaskCycle : public mip::Group {
public:
	using SharedPtr = std::shared_ptr<GroupTaskCycle>;

public:
	explicit GroupTaskCycle(
		YannickProblem const& yannick_problem,
		GroupTaskTime const& group_task_time
	);

	void create_variables_constraints_and_objective(mip::MIPModel& mip_model) final;

	json compute_solution() const final;

	mip::VariableStorage<graph::NodeId, Cycle> const& variables() const;

private:
	void create_variables(mip::MIPModel& mip_model);
	void create_constraints(mip::MIPModel& mip_model);

private:
	YannickProblem const& _yannick_problem;
	GroupTaskTime const& _group_task_time;

	mip::VariableStorage<graph::NodeId, Cycle> _variables;
};

} // namespace yannick

#endif
