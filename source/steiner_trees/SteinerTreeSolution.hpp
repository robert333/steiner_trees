#ifndef STEINER_TREES_STEINERTREESOLUTION_HPP
#define STEINER_TREES_STEINERTREESOLUTION_HPP

#include "../json.hpp"
#include "../graph/typedefs.hpp"
#include "SteinerTreeProblem.hpp"

namespace steiner_trees {

class SteinerTreeSolution {
public:
	using SharedPtr = std::shared_ptr<SteinerTreeSolution>;
	using Vector = std::vector<SteinerTreeSolution>;

public:
	explicit SteinerTreeSolution(
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

} // namespace steiner_trees

#endif
