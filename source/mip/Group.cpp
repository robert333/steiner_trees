#include "Group.hpp"

namespace mip {

Group::Group(Name const& name) :
	_name(name)
{}

Group::Name const& Group::name() const
{
	return _name;
}

json Group::compute_solution() const
{
	return {};
}

} // namespace mip
