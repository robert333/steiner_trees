#ifndef SKUTELLA_SKUTELLAMIPFACTORY_HPP
#define SKUTELLA_SKUTELLAMIPFACTORY_HPP

#include "../mip/GroupManager.hpp"
#include "SkutellaMIP.hpp"

namespace skutella {

class SkutellaMIPFactory {
public:
	static mip::GroupManager create(SkutellaProblem const& skutella_problem);

private:

};

} // namespace skutella

#endif
