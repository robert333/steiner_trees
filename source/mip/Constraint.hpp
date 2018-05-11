#ifndef MIP_CONSTRAINT_HPP
#define MIP_CONSTRAINT_HPP

#include <ortools/linear_solver/linear_solver.h>

namespace mip {

class Constraint {
public:
//	using Vector = std::vector<Constraint>;

	enum BoundType {
		LOWER_BOUND,
		UPPER_BOUND
	};

public:
	explicit Constraint(operations_research::MPConstraint* mp_constraint_ptr = nullptr);

	bool is_valid() const;

	std::string name() const;

	void add_variable(operations_research::MPVariable* mp_variable_ptr, double coefficient);

	void set_lower_bound(double lower_bound);
	void set_upper_bound(double upper_bound);

	double lower_bound() const;
	double upper_bound() const;

	double compute_valid_bound(BoundType bound_type) const;
	double compute_valid_lower_bound() const;
	double compute_valid_upper_bound() const;

	bool is_lower_bound_trivially_fulfilled() const;
	bool is_upper_bound_trivially_fulfilled() const;
	bool is_trivially_fulfilled() const;

	void lower_bound_condition_on(operations_research::MPVariable* mp_variable_ptr, double value = 1);
	void upper_bound_condition_on(operations_research::MPVariable* mp_variable_ptr, double value = 1);

private:
	operations_research::MPConstraint& mp_constraint();
	operations_research::MPConstraint const& mp_constraint() const;

private:
	operations_research::MPConstraint* _mp_constraint_ptr;

	std::vector<operations_research::MPVariable*> _variables;
};

} // namespace mip

#endif
