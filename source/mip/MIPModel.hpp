#ifndef MIP_MIPMODEL_HPP
#define MIP_MIPMODEL_HPP

#include <ortools/linear_solver/linear_solver.h>

#include "Constraint.hpp"

namespace mip {

using Value = double;

class MIPModel {
public:
	using Variable = operations_research::MPVariable;
	using MPConstraint = operations_research::MPConstraint;
	using Objective = operations_research::MPObjective;

public:
	explicit MIPModel() = default;

	virtual Variable* create_continuous_variable(
		std::string const& group,
		std::string const& name,
		double lower_bound,
		double upper_bound
	) = 0;

	virtual Variable* create_integer_variable(
		std::string const& group,
		std::string const& name,
		double lower_bound,
		double upper_bound
	) = 0;

	virtual Variable* create_binary_variable(
		std::string const& group,
		std::string const& name
	) = 0;

	virtual Constraint create_constraint(
		std::string const& group,
		std::string const& name
	) = 0;

	virtual void set_objective_coefficient(Variable const* variable, double coefficient) = 0;
	virtual void add_objective_offset(double offset) = 0;

	virtual void output(std::ostream& ostream) = 0;
};

} // namespace mip

#endif
