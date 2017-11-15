#ifndef YANNICK_YANNICKSOLUTION_HPP
#define YANNICK_YANNICKSOLUTION_HPP

#include "../json.hpp"
#include "../graph/typedefs.hpp"
#include "../mip/MIP.hpp"

#include "YannickProblem.hpp"

namespace yannick {

class YannickSolution {
public:
	using SharedPtr = std::shared_ptr<YannickSolution>;
	using Vector = std::vector<YannickSolution>;

public:
	explicit YannickSolution(
		mip::MIP::OptimizationProblem const& optimization_problem,
		mip::MIP::OptimizationType const& optimization_type,
		mip::MIP::OptimizationResult const& optimization_result,
		mip::MIP::Value const& optimization_value,
		json const& optimization_solution
	);

	mip::MIP::OptimizationProblem const& optimization_problem() const;
	mip::MIP::OptimizationType const& optimization_type() const;
	mip::MIP::OptimizationResult const& optimization_result() const;
	mip::MIP::Value const& optimization_value() const;
	json const& optimization_solution() const;

	json export_to_json() const;

private:

private:
	mip::MIP::OptimizationProblem const _optimization_problem;
	mip::MIP::OptimizationType const _optimization_type;
	mip::MIP::OptimizationResult const _optimization_result;
	mip::MIP::Value const _optimization_value;

	json const _optimization_solution;
};

} // namespace yannick

#endif
