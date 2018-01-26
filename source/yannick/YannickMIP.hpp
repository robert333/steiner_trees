#ifndef YANNICK_YANNICKMIP_HPP
#define YANNICK_YANNICKMIP_HPP

#include "YannickSolution.hpp"
#include "YannickProblem.hpp"

namespace yannick {

class YannickMIP {
public:
	static YannickSolution solve(
		YannickProblem const& yannick_problem,
		mip::MIP::OptimizationProblem const& optimization_problem
	);

private:
	explicit YannickMIP() = default;

private:
};

} // namespace yannick

#endif
