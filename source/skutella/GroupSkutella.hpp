#ifndef SKUTELLA_GROUPSKUTELLA_HPP
#define SKUTELLA_GROUPSKUTELLA_HPP

#include "../mip/Group.hpp"
#include "../mip/VariableStorage.hpp"
#include "../graph/Graph.hpp"
#include "../graph/Net.hpp"
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

	mip::VariableStorage<Number, Number> _set_variables;
	mip::VariableStorage<std::vector<Number>, Number> _set_cover_variables;
	mip::VariableStorage<Number> _path_variable;
};

} // namespace skutella

#endif
