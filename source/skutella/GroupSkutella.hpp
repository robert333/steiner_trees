#ifndef SKUTELLA_GROUPSKUTELLA_HPP
#define SKUTELLA_GROUPSKUTELLA_HPP

#include "../mip/Group.hpp"
#include "../mip/VariableStorage.hpp"
#include "../graph/Graph.hpp"
#include "../graph/Net.hpp"
#include "../helper/PowerSetIterator.hpp"
#include "SkutellaProblem.hpp"

namespace skutella {

class GroupSkutella : public mip::Group {
public:
	using SharedPtr = std::shared_ptr<GroupSkutella>;

public:
	explicit GroupSkutella(SkutellaProblem const& skutella_problem);

	void create_variables_constraints_and_objective(mip::MIPModel& mip_model) final;

	json compute_solution() const final;

private:
	void create_variables(mip::MIPModel& mip_model);
	void create_constraints(mip::MIPModel& mip_model);
	void create_objective(mip::MIPModel& mip_model);

private:
	SkutellaProblem const& _skutella_problem;

	mip::VariableStorage<Index, Index> _set_variables;
	mip::VariableStorage<helper::PowerSetIterator::Set, Index> _set_cover_variables;
	mip::VariableStorage<Index> _path_variable;
};

} // namespace skutella

#endif
