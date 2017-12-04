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

std::size_t MIP::num_variables(std::string const& group) const
{
	return _num_variables_per_group.at(group);
}

std::size_t MIP::num_constraints(std::string const& group) const
{
	return _num_constraints_per_group.at(group);
}

std::vector<MIP::Variable*> const& MIP::variables() const
{
	return solver().variables();
}

std::vector<MIP::MPConstraint*> const& MIP::constraints() const
{
	return solver().constraints();
}

MIP::Variable* MIP::create_continuous_variable(
	std::string const& group,
	std::string const& name,
	double lower_bound,
	double upper_bound
)
{
	++_num_variables_per_group[group];
	return solver().MakeNumVar(lower_bound, upper_bound, name);
}

MIP::Variable* MIP::create_integer_variable(
	std::string const& group,
	std::string const& name,
	double lower_bound,
	double upper_bound
)
{
	++_num_variables_per_group[group];
	return solver().MakeIntVar(lower_bound, upper_bound, name);
}

MIP::Variable* MIP::create_binary_variable(
	std::string const& group,
	std::string const& name
)
{
	++_num_variables_per_group[group];
	return solver().MakeBoolVar(name);
}

Constraint MIP::create_constraint(
	std::string const& group,
	std::string const& name
)
{
	++_num_constraints_per_group[group];
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

Value MIP::objective_value() const
{
	return solver().Objective().Value();
}

void MIP::output_statistics(std::ostream& ostream) const
{
	ostream << "[MIP] " << solver().Name() << "\n"
			<< "[MIP] num variables   = " << num_variables() << "\n"
			<< "[MIP] num constraints = " << num_constraints() << "\n";

	for (auto const& pair : _num_variables_per_group) {
		ostream << "[MIP] " << pair.first << "\n"
				<< "[MIP] num variables   = " << num_variables(pair.first) << "\n";
	}

	for (auto const& pair : _num_constraints_per_group) {
		ostream << "[MIP] " << pair.first << "\n"
				<< "[MIP] num constraints = " << num_constraints(pair.first) << "\n";
	}
}

void MIP::output(std::ostream& ostream)
{
	std::string lp_string;
	solver().ExportModelAsLpFormat(false, &lp_string);
	ostream << lp_string;
}

} // namespace mip
