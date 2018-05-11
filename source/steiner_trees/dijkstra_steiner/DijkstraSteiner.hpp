#ifndef STEINERTREES_DIJKSTRASTEINER_HPP
#define STEINERTREES_DIJKSTRASTEINER_HPP

#include <steiner_trees/SteinerTreeProblem.hpp>
#include <steiner_trees/SteinerTreeSolution.hpp>

namespace steiner_trees {

class DijkstraSteiner {
public:
	static SteinerTreeSolution solve(SteinerTreeProblem const& steiner_tree_problem);

public:
	explicit DijkstraSteiner() = default;
};

} // namespace steiner_trees

#endif
