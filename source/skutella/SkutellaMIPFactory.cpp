#include "SkutellaMIPFactory.hpp"

#include "GroupSkutella.hpp"

namespace skutella {

mip::GroupManager SkutellaMIPFactory::create(SkutellaProblem const& skutella_problem)
{
	GroupSkutella::SharedPtr group_skutella(
		std::make_shared<GroupSkutella>(skutella_problem)
	);

	mip::GroupManager group_manager{};

	group_manager.add(group_skutella);

	return group_manager;
}

} // namespace skutella
