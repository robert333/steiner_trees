#ifndef YANNICK_YANNICKSOLVER_HPP
#define YANNICK_YANNICKSOLVER_HPP

#include "YannickProblem.hpp"
#include "YannickSolution.hpp"

namespace yannick {

class YannickSolver {
public:
	using SharedPtr = std::shared_ptr<YannickSolver>;
	using Vector = std::vector<YannickSolver>;

public:
	static YannickSolution solve(YannickProblem const& yannick_problem);

private:
	explicit YannickSolver() = default;

private:
};

} // namespace yannick

#endif
