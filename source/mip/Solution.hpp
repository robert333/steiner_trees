#ifndef MIP_SOLUTION_HPP
#define MIP_SOLUTION_HPP

#include "../json.hpp"
#include "MIP.hpp"

namespace mip {

class Solution {
public:
	using SharedPtr = std::shared_ptr<Solution>;
	using Vector = std::vector<Solution>;

public:
	explicit Solution(
		MIP::OptimizationProblem const& optimization_problem,
		MIP::OptimizationType const& optimization_type,
		MIP::OptimizationResult const& optimization_result,
		Value const& optimization_value,
		json const& optimization_solution
	);

	MIP::OptimizationProblem const& optimization_problem() const;
	MIP::OptimizationType const& optimization_type() const;
	MIP::OptimizationResult const& optimization_result() const;
	Value const& optimization_value() const;
	json const& optimization_solution() const;

	json export_to_json() const;

private:

private:
	MIP::OptimizationProblem const _optimization_problem;
	MIP::OptimizationType const _optimization_type;
	MIP::OptimizationResult const _optimization_result;
	Value const _optimization_value;

	json const _optimization_solution;
};

} // namespace mip

#endif
