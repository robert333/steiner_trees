#ifndef YANNICK_YANNICKMIPFACTORY_HPP
#define YANNICK_YANNICKMIPFACTORY_HPP

#include "../mip/GroupManager.hpp"
#include "YannickMIP.hpp"

namespace yannick {

class YannickMIPFactory {
public:
	static mip::GroupManager create(YannickProblem const& yannick_problem);

	static mip::GroupManager create_nmc(YannickProblem const& yannick_problem);
	static mip::GroupManager create_dcb(YannickProblem const& yannick_problem);

private:

};

} // namespace yannick

#endif
