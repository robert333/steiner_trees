#ifndef YANNICK_GROUPCYCLE_HPP
#define YANNICK_GROUPCYCLE_HPP

#include "../mip/Group.hpp"
#include "../mip/VariableStorage.hpp"
#include "../graph/Graph.hpp"
#include "../graph/Net.hpp"
#include "YannickProblem.hpp"
#include "GroupTaskCycle.hpp"

namespace yannick {

class GroupCycle : public mip::Group {
public:
	using SharedPtr = std::shared_ptr<GroupCycle>;

public:
	explicit GroupCycle(
		YannickProblem const& yannick_problem,
		GroupTaskCycle const& group_task_cycle
	);

	void create_variables_constraints_and_objective(mip::MIPModel& mip_model) final;

	json compute_solution() const final;

	mip::VariableStorage<Cycle> const& variables() const;

private:
	void create_variables(mip::MIPModel& mip_model);
	void create_constraints(mip::MIPModel& mip_model);
	void create_objective(mip::MIPModel& mip_model);

private:
	YannickProblem const& _yannick_problem;
	GroupTaskCycle const& _group_task_cycle;

	mip::VariableStorage<Cycle> _variables;
};

} // namespace yannick

#endif
