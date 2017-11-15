#ifndef YANNICK_GROUPTASKTIME_HPP
#define YANNICK_GROUPTASKTIME_HPP

#include "../mip/Group.hpp"
#include "../mip/VariableStorage.hpp"
#include "../graph/Graph.hpp"
#include "../graph/Net.hpp"
#include "YannickProblem.hpp"

namespace yannick {

class GroupTaskTime : public mip::Group {
public:
	using SharedPtr = std::shared_ptr<GroupTaskTime>;

public:
	explicit GroupTaskTime(YannickProblem const& yannick_problem);

	void create_variables_constraints_and_objective(mip::MIPModel& mip_model) final;

	json compute_solution() const final;

	mip::VariableStorage<graph::NodeId> const& variables() const;

private:
	void create_variables(mip::MIPModel& mip_model);
	void create_constraints(mip::MIPModel& mip_model);

private:
	YannickProblem const& _yannick_problem;

	mip::VariableStorage<graph::NodeId> _variables;
};

} // namespace yannick

#endif
