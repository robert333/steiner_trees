#ifndef SKUTELLA_SKUTELLASOLVER_HPP
#define SKUTELLA_SKUTELLASOLVER_HPP

#include "SkutellaProblem.hpp"
#include "SkutellaSolution.hpp"

namespace skutella {

class SkutellaSolver {
public:
	using SharedPtr = std::shared_ptr<SkutellaSolver>;
	using Vector = std::vector<SkutellaSolver>;

public:
	static SkutellaSolution solve(SkutellaProblem const& skutella_problem);

private:
	explicit SkutellaSolver() = default;

private:
};

} // namespace skutella

#endif
