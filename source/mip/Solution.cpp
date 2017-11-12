#include "Solution.hpp"

#include "../helper/map.hpp"

namespace mip {

void Solution::set(std::string const& group_name, json const& group_solution)
{
	assert(not helper::exists(_solutions, group_name));
	_solutions[group_name] = group_solution;
}

json Solution::get(std::string const& group_name) const
{
	assert(helper::exists(_solutions, group_name));
	return _solutions.at(group_name);
}

} // namespace mip
