#ifndef STEINER_TREES_OPTIMALINTEGRALITYGAP_HPP
#define STEINER_TREES_OPTIMALINTEGRALITYGAP_HPP

#include <graph/TerminalInstance.hpp>

namespace steiner_trees {

class OptimalIntegralityGap {
public:
	static std::map<graph::EdgeId, graph::Weight> compute_optimal_integrality_gap(
		graph::TerminalInstance const& terminal_instance
	);

private:
	static std::map<graph::EdgeId, graph::Weight>

private:
	explicit OptimalIntegralityGap() = default;
};

} // namespace steiner_trees

#endif
