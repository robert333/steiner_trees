#ifndef SKUTELLA_SKUTELLAMIP_HPP
#define SKUTELLA_SKUTELLAMIP_HPP

#include "SkutellaProblem.hpp"
#include "SkutellaSolution.hpp"

namespace skutella {

class SkutellaMIP {
public:
	static SkutellaSolution solve(
		SkutellaProblem const& skutella_problem,
		mip::MIP::OptimizationProblem const& optimization_problem
	);

private:
	explicit SkutellaMIP() = default;

private:
};

} // namespace skutella

#endif
