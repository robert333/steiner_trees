#include "MIP.hpp"

namespace mip {

MIP::MIP(std::string const& name, OptimizationProblem const& optimization_problem) :
	MIPSolver(name, optimization_problem)
{}

std::size_t MIP::num_variables() const
{
	return variables().size();
}

std::size_t MIP::num_constraints() const
{
	return constraints().size();
}

std::vector<MIP::Variable*> const& MIP::variables() const
{
	return solver().variables();
}

std::vector<MIP::MPConstraint*> const& MIP::constraints() const
{
	return solver().constraints();
}

MIP::Variable* MIP::create_continuous_variable(std::string const& name, double lower_bound, double upper_bound)
{
	return solver().MakeNumVar(lower_bound, upper_bound, name);
}

MIP::Variable* MIP::create_integer_variable(std::string const& name, double lower_bound, double upper_bound)
{
	return solver().MakeIntVar(lower_bound, upper_bound, name);
}

MIP::Variable* MIP::create_binary_variable(std::string const& name)
{
	return solver().MakeBoolVar(name);
}

Constraint MIP::create_constraint(std::string const& name)
{
	return Constraint(solver().MakeRowConstraint(name));
}

void MIP::set_objective_coefficient(MIP::Variable const* variable, double coefficient)
{
	solver().MutableObjective()->SetCoefficient(variable, coefficient);
}

void MIP::add_objective_offset(double offset)
{
	solver().MutableObjective()->SetOffset(solver().Objective().offset() + offset);
}

MIPModel::Value MIP::objective_value() const
{
	return solver().Objective().Value();
}

void MIP::output(std::ostream& ostream)
{
	std::string lp_string;
	solver().ExportModelAsLpFormat(false, &lp_string);
	ostream << lp_string;
}

} // namespace mip
