#ifndef MIP_MIP_HPP
#define MIP_MIP_HPP

#include "MIPModel.hpp"
#include "MIPSolver.hpp"

namespace mip {

class MIP : public MIPModel, public MIPSolver {
public:

public:
	explicit MIP(std::string const& name, OptimizationProblem const& optimization_problem);

	std::size_t num_variables() const;
	std::size_t num_constraints() const;

	std::size_t num_variables(std::string const& group) const;
	std::size_t num_constraints(std::string const& group) const;

	std::vector<Variable*> const& variables() const;
	std::vector<MPConstraint*> const& constraints() const;

	Variable* create_continuous_variable(
		std::string const& group,
		std::string const& name,
		double lower_bound,
		double upper_bound
	) final;

	Variable* create_integer_variable(
		std::string const& group,
		std::string const& name,
		double lower_bound,
		double upper_bound
	) final;

	Variable* create_binary_variable(
		std::string const& group,
		std::string const& name
	) final;

	Constraint create_constraint(
		std::string const& group,
		std::string const& name
	) final;

	void set_objective_coefficient(Variable const* variable, double coefficient) final;
	void add_objective_offset(double offset) final;

	Value objective_value() const;

	void output_statistics(std::ostream& ostream) const;

	void output(std::ostream& ostream) final;

private:

private:
	std::map<std::string, std::size_t> _num_variables_per_group;
	std::map<std::string, std::size_t> _num_constraints_per_group;
};

} // namespace mip

#endif
