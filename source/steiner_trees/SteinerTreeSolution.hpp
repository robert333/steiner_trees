#ifndef STEINER_TREES_STEINERTREESOLUTION_HPP
#define STEINER_TREES_STEINERTREESOLUTION_HPP

#include "../json.hpp"
#include "../graph/typedefs.hpp"
#include "../mip/Solution.hpp"

namespace steiner_trees {

class SteinerTreeSolution {
public:
	using SharedPtr = std::shared_ptr<SteinerTreeSolution>;
	using Vector = std::vector<SteinerTreeSolution>;

public:
	explicit SteinerTreeSolution(
		graph::Weight const& optimum_value,
		json const& solution = {}
	);

	graph::Weight const& optimum_value() const;
	json const& solution() const;

private:

private:
	graph::Weight _optimum_value;
	json _solution;
};

} // namespace steiner_trees

#endif
