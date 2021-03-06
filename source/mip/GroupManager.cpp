#include "GroupManager.hpp"

#include "../helper/vector.hpp"
#include "../helper/map.hpp"
#include "../Logger.hpp"

namespace mip {

void GroupManager::add(Group::SharedPtr const& group_ptr)
{
	assert(not helper::contains(_group_order, group_ptr->name()));
	assert(not helper::exists(_groups, group_ptr->name()));
	_group_order.push_back(group_ptr->name());
	_groups[group_ptr->name()] = group_ptr;
}

void GroupManager::remove(Group::SharedPtr const& group_ptr)
{
	assert(helper::contains(_group_order, group_ptr->name()));
	assert(helper::exists(_groups, group_ptr->name()));
	helper::erase(_group_order, group_ptr->name());
	_groups.erase(group_ptr->name());
}

void GroupManager::create_variables_constraints_and_objective(MIPModel& mip_model) const
{
	for (std::string const& group_name : _group_order) {
		Logger::logger() << "[GroupManager] add group " << group_name << "\n";
		_groups.at(group_name)->create_variables_constraints_and_objective(mip_model);
	}
}

json GroupManager::compute_solutions() const
{
	json solutions;

	for (std::string const& group_name : _group_order) {
		Logger::logger() << "[GroupManager] compute solution for group " << group_name << "\n";
		solutions[group_name] = _groups.at(group_name)->compute_solution();
	}

	return solutions;
}

} // namespace mip
