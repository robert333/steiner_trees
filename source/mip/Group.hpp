#ifndef MIP_GROUP_HPP
#define MIP_GROUP_HPP

#include "MIPModel.hpp"
#include "../json.hpp"

namespace mip {

class Group {
public:
	using SharedPtr = std::shared_ptr<Group>;
	using Name = std::string;

public:
	explicit Group(Name const& name);

	Name const& name() const;

	virtual void create_variables_constraints_and_objective(MIPModel& mip_model) = 0;

	virtual json compute_solution() const;

private:

private:
	Name _name;
};

} // namespace mip

#endif
