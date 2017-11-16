#include "Constraint.hpp"

namespace mip {

Constraint::Constraint(operations_research::MPConstraint* mp_constraint_ptr) :
	_mp_constraint_ptr(mp_constraint_ptr)
{}

bool Constraint::is_valid() const
{
	return _mp_constraint_ptr != nullptr;
}

std::string Constraint::name() const
{
	return mp_constraint().name();
}

void Constraint::add_variable(operations_research::MPVariable* mp_variable_ptr, double coefficient)
{
	mp_constraint().SetCoefficient(mp_variable_ptr, coefficient);

	_variables.push_back(mp_variable_ptr);
}

void Constraint::set_lower_bound(double lower_bound)
{
	mp_constraint().SetLB(lower_bound);
}

void Constraint::set_upper_bound(double upper_bound)
{
	mp_constraint().SetUB(upper_bound);
}

double Constraint::lower_bound() const
{
	return mp_constraint().lb();
}

double Constraint::upper_bound() const
{
	return mp_constraint().ub();
}

double Constraint::compute_valid_bound(BoundType bound_type) const
{
	double bound = 0;

	for (operations_research::MPVariable const* mp_variable_ptr : _variables) {
		double const coefficient = mp_constraint().GetCoefficient(mp_variable_ptr);

		if ((bound_type == LOWER_BOUND ? 1 : -1) * mp_constraint().GetCoefficient(mp_variable_ptr) >= 0) {
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
		mp_constraint().SetLB(valid_lower_bound);
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
		mp_constraint().SetUB(valid_upper_bound);
	}

	add_variable(mp_variable_ptr, coefficient);
}

operations_research::MPConstraint& Constraint::mp_constraint()
{
	assert(is_valid());
	return *_mp_constraint_ptr;
}

operations_research::MPConstraint const& Constraint::mp_constraint() const
{
	assert(is_valid());
	return *_mp_constraint_ptr;
}

} // namespace mip
