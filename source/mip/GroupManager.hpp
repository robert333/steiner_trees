#ifndef MIP_GROUP_MANAGER_HPP
#define MIP_GROUP_MANAGER_HPP

#include "Group.hpp"
#include "Solution.hpp"

namespace mip {

class GroupManager {
public:
	using Solution = std::map<std::string, json>;

public:
	explicit GroupManager() = default;

	void add(Group::SharedPtr const& group_ptr);
	void remove(Group::SharedPtr const& group_ptr);

	void create_variables_constraints_and_objective(MIPModel& mip_model) const;

	json compute_solutions() const;

private:

private:
	std::vector<Group::Name> _group_order;
	std::map<Group::Name, Group::SharedPtr> _groups;
};

} // namespace mip

#endif
