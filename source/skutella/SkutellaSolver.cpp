#include "SkutellaSolver.hpp"

#include "SkutellaMIP.hpp"

namespace skutella {

SkutellaSolution SkutellaSolver::solve(SkutellaProblem const& skutella_problem)
{
	std::cout << "[SkutellaSolver] solve...\n";
	return SkutellaMIP::solve(skutella_problem, mip::MIP::MIXED_INTEGER_PROGRAMMING);
}

} // namespace skutella
