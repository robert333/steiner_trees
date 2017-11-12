#ifndef MIP_MIP_SOLVER_HPP
#define MIP_MIP_SOLVER_HPP

#include "../../or-tools-6.4/ortools/linear_solver/linear_solver.h"

namespace mip {

class MIPSolver {
public:
	using Solver = operations_research::MPSolver;

	enum OptimizationProblem {
		LINEAR_PROGRAMMING,
		MIXED_INTEGER_PROGRAMMING
	};

	enum OptimizationType {
		MINIMIZATION,
		MAXIMIZATION
	};

	using OptimizationResult = Solver::ResultStatus;

	static std::string to_string(OptimizationProblem const& optimization_problem);
	static std::string to_string(OptimizationType const& optimization_type);

private:
	static Solver::OptimizationProblemType get_solver_optimization_problem_type(
		OptimizationProblem const& optimization_problem
	);

public:
	explicit MIPSolver(std::string const& name, OptimizationProblem const& optimization_problem);

	void set_optimization_type(OptimizationType const& optimization_type);

	Solver::ResultStatus optimize();

protected:
	Solver& solver();
	Solver const& solver() const;

private:
	std::shared_ptr<Solver> _solver;
};

} // namespace mip

#endif
