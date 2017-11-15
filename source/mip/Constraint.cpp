#include "Constraint.hpp"

namespace mip {

Constraint::Constraint(operations_research::MPConstraint* mp_constraint_ptr) :
	_mp_constraint_ptr(mp_constraint_ptr)
{}

void Constraint::add_variable(operations_research::MPVariable* mp_variable_ptr, double coefficient)
{
	_mp_constraint_ptr->SetCoefficient(mp_variable_ptr, coefficient);

	_variables.push_back(mp_variable_ptr);
}

void Constraint::set_lower_bound(double lower_bound)
{
	_mp_constraint_ptr->SetLB(lower_bound);
}

void Constraint::set_upper_bound(double upper_bound)
{
	_mp_constraint_ptr->SetUB(upper_bound);
}

double Constraint::lower_bound() const
{
	return _mp_constraint_ptr->lb();
}

double Constraint::upper_bound() const
{
	return _mp_constraint_ptr->ub();
}

double Constraint::compute_valid_bound(BoundType bound_type) const
{
	double bound = 0;

	for (operations_research::MPVariable const* mp_variable_ptr : _variables) {
		double const coefficient = _mp_constraint_ptr->GetCoefficient(mp_variable_ptr);

		if ((bound_type == LOWER_BOUND ? 1 : -1) * _mp_constraint_ptr->GetCoefficient(mp_variable_ptr) >= 0) {
			bound += coefficient * mp_variable_ptr->lb();
		} else {
			bound += coefficient * mp_variable_ptr->ub();
		}
	}

	return bound;
}

double Constraint::compute_valid_lower_bound() const
{
	return compute_valid_bound(LOWER_BOUND);
}

double Constraint::compute_valid_upper_bound() const
{
	return compute_valid_bound(UPPER_BOUND);
}

bool Constraint::is_lower_bound_trivially_fulfilled() const
{
	return lower_bound() <= compute_valid_lower_bound();
}

bool Constraint::is_upper_bound_trivially_fulfilled() const
{
	return upper_bound() >= compute_valid_upper_bound();
}

bool Constraint::is_trivially_fulfilled() const
{
	return is_lower_bound_trivially_fulfilled() and is_upper_bound_trivially_fulfilled();
}

void Constraint::lower_bound_condition_on(operations_research::MPVariable* mp_variable_ptr, double value)
{
	assert(mp_variable_ptr->integer());
	assert(mp_variable_ptr->lb() == 0);
	assert(mp_variable_ptr->ub() == 1);
	assert(value == 0 or value == 1);

	if (is_lower_bound_trivially_fulfilled()) {
		return;
	}

	double const valid_lower_bound = compute_valid_lower_bound();
	double const coefficient = (value == 0 ? 1 : -1) * (lower_bound() - valid_lower_bound);

	if (value == 1) {
		_mp_constraint_ptr->SetLB(valid_lower_bound);
	}

	add_variable(mp_variable_ptr, coefficient);
}

void Constraint::upper_bound_condition_on(operations_research::MPVariable* mp_variable_ptr, double value)
{
	assert(mp_variable_ptr->integer());
	assert(mp_variable_ptr->lb() == 0);
	assert(mp_variable_ptr->ub() == 1);
	assert(value == 0 or value == 1);

	if (is_upper_bound_trivially_fulfilled()) {
		return;
	}

	double const valid_upper_bound = compute_valid_upper_bound();
	double const coefficient = (value == 0 ? 1 : -1) * (upper_bound() - valid_upper_bound);

	if (value == 1) {
		_mp_constraint_ptr->SetUB(valid_upper_bound);
	}

	add_variable(mp_variable_ptr, coefficient);
}

} // namespace mip
