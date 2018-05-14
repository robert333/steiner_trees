#include "DijkstraSteiner.hpp"

#include <steiner_trees/dijkstra_steiner/DijkstraSteinerAlgorithm.hpp>

static constexpr std::size_t DIJKSTRA_STEINER_MAX_TERMINALS = 20;

namespace steiner_trees {

SteinerTreeSolution DijkstraSteiner::solve(SteinerTreeProblem const& steiner_tree_problem)
{
	graph::TerminalInstance const& terminal_instance = steiner_tree_problem.terminal_instance();

	debug_output("\n== Dijkstra-Steiner Algorithm ==\n\n");

	dijkstra_steiner::DijkstraSteinerAlgorithm<int, DIJKSTRA_STEINER_MAX_TERMINALS> dijkstra_steiner_algorithm(
		steiner_tree_problem.terminal_instance()
	);

	debug_output("\nPreparation\n");

	dijkstra_steiner_algorithm.preparation();

	debug_output("\nRun\n");

	graph::Weight const optimum_value = dijkstra_steiner_algorithm.run();

	debug_output("\nBacktrack\n");

	dijkstra_steiner::SteinerTreeKey<DIJKSTRA_STEINER_MAX_TERMINALS> const root_terminal_key(
		terminal_instance.root_terminal(),
		BitSet<20>(terminal_instance.terminals().size() - 1).flip()
	);

	std::vector<graph::EdgeId> const edge_ids = dijkstra_steiner_algorithm.backtrack(root_terminal_key);

	json solution;

	for (graph::EdgeId const& edge_id : edge_ids) {
		graph::Edge const& edge = terminal_instance.undirected_graph().edge(edge_id);
		solution["edges"].push_back(
			{
				{"tail", edge.tail()},
				{"head", edge.head()}
			}
		);
	}

	debug_output("\n================================\n");

	return SteinerTreeSolution(optimum_value, solution);
}

} // namespace steiner_trees
