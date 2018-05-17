#include "steiner_trees.hpp"

#include <fstream>

#include "graph/GraphFactory.hpp"
#include "graph/GraphPrinter.hpp"
#include "graph/NetsFactory.hpp"

#include "steiner_trees/SteinerTreeSolver.hpp"

#include "Logger.hpp"

void run_steiner_trees(
	std::string const& program_path,
	std::string const& instance_path,
	std::string const& instance_name
)
{
	std::ofstream log(instance_path + "/" + instance_name + ".log");

	Logger::set_logger(std::cout);

	Logger::logger() << "Steiner Tree LP/MIP Solver\n"
					 << "\n"
					 << "program path  : " << program_path << "\n"
					 << "instance path : " << instance_path << "\n"
					 << "instance name : " << instance_name << "\n"
					 << "\n";

	std::string const instance_file_path = instance_path + "/" + instance_name + ".json";

	json const instance_json = read_json(instance_file_path);

	graph::Graph const graph = graph::GraphFactory::create_from_json(instance_json["graph"]);
	graph::Net::Vector const nets = graph::NetsFactory::create_from_json(instance_json["nets"]);

	assert(nets.size() == 1);

	graph::TerminalInstance const terminal_instance(graph, nets.front().terminals());

	steiner_trees::SteinerTreeProblem const steiner_tree_problem("test", terminal_instance, graph, nets);

	json const analysis = analyze_bidirected_cut_relaxation(steiner_tree_problem);

	std::string analysis_file_path = instance_path + "/analysis.json";

	write_json(analysis_file_path, analysis);

	if (terminal_instance.terminals().size() <= 4) {
		if (analysis.count("simplex_embedding") > 0) {
			if (analysis["simplex_embedding"]["lambda"] <= 30) {
				std::string const plot_simplex_embedding_command =
					"python3 /home/robert/Documents/studies/discrete_mathematics/n-simplex/simplex_embedding.py "
					+ instance_file_path + " " + analysis_file_path;// + " " + instance_path;

				Logger::logger() << "execute: " << plot_simplex_embedding_command << "\n";

				int const status = system(plot_simplex_embedding_command.c_str());

				CRITICAL_CHECK(status == 0, "system error")
			} else {
				Logger::logger() << "We do not plot the solution as the size of the simplex is too large, "
								 << "size = " << analysis["simplex_embedding"]["lambda"] << " > 30";
			}
		} else {
			Logger::logger() << "We do not plot the solution as we did not compute a simplex embedding";
		}
	}
}

json analyze_bidirected_cut_relaxation(steiner_trees::SteinerTreeProblem const& steiner_tree_problem)
{
	graph::TerminalInstance const& terminal_instance = steiner_tree_problem.terminal_instance();
	graph::Graph const& graph = terminal_instance.undirected_graph();

	std::size_t const num_terminals = terminal_instance.terminals().size();

	Logger::logger() << "Steiner Tree BCR Analysis\n"
					 << "\n"
					 << "num nodes     : " << graph.num_nodes() << "\n"
					 << "num edges     : " << graph.num_edges() << "\n"
					 << "num terminals : " << num_terminals << "\n"
					 << "root terminal : " << terminal_instance.root_terminal() << "\n"
					 << "\n";

	std::optional<steiner_trees::SteinerTreeSolution> solution_simplex_embedding;
	std::optional<steiner_trees::SteinerTreeSolution> solution_bmccf;
	std::optional<steiner_trees::SteinerTreeSolution> solution_emcf_integer;

	auto const solution_emcf_continuous = steiner_trees::SteinerTreeSolver::solve_via_linear_programming(
		steiner_tree_problem,
		steiner_trees::SteinerTreeMIP::EMCF,
		mip::MIP::LINEAR_PROGRAMMING
	);

	if (num_terminals <= 4 and graph.num_edges() < 10000) {
		solution_simplex_embedding = steiner_trees::SteinerTreeSolver::solve_via_linear_programming(
			steiner_tree_problem,
			steiner_trees::SteinerTreeMIP::SIMPLEX_EMBEDDING,
			mip::MIP::LINEAR_PROGRAMMING
		);

		if (num_terminals == 3) {
//			solution_bmccf = steiner_trees::SteinerTreeSolver::solve_via_linear_programming(
//				steiner_tree_problem,
//				steiner_trees::SteinerTreeMIP::BIDIRECTED_MULTI_COMMODITY_COMMON_FLOW,
//				mip::MIP::MIXED_INTEGER_PROGRAMMING
//			);
		}
	}

//	auto const solution_emcf_integer = steiner_trees::SteinerTreeSolver::solve_via_linear_programming(
//		steiner_tree_problem,
//		steiner_trees::SteinerTreeMIP::EMCF,
//		mip::MIP::MIXED_INTEGER_PROGRAMMING
//	);

	auto const solution_dijkstra_steiner = steiner_trees::SteinerTreeSolver::solve_via_combinatorial_optimization(
		steiner_tree_problem
	);

	mip::Value const optimum_value_emcf_continuous = solution_emcf_continuous.optimum_value();
	mip::Value const optimum_value_dijkstra_steiner = solution_dijkstra_steiner.optimum_value();
	mip::Value const integrality_gap = optimum_value_dijkstra_steiner / optimum_value_emcf_continuous;

	Logger::logger() << "optimum value EMCF continuous   = " << optimum_value_emcf_continuous << "\n";
	Logger::logger() << "optimum value Dijkstra Steiner  = " << optimum_value_dijkstra_steiner << "\n";
	Logger::logger() << "integrality gap EMCF            = " << integrality_gap << "\n";

	json analysis = {
		{"emcf_edges",       solution_emcf_continuous.solution()["variables"]["GroupEdges"]["bidirected"]},
		{"optimum_solution", solution_dijkstra_steiner.solution()}
	};

	if (solution_simplex_embedding.has_value()) {
		mip::Value const optimum_value_simplex_embedding = solution_simplex_embedding->optimum_value();
		std::cout << "optimum value simplex embedding = " << optimum_value_simplex_embedding << "\n";
		CRITICAL_CHECK(
			optimum_value_emcf_continuous == optimum_value_simplex_embedding,
			"EMCF (LP) and Simplex Embedding (LP) should have the same optimum value, error = "
			+ std::to_string(std::abs(optimum_value_emcf_continuous - optimum_value_simplex_embedding))
		)
		analysis["simplex_embedding"] = solution_simplex_embedding->solution()["variables"]["GroupSimplexEmbedding"];
	}

	if (solution_bmccf.has_value()) {
		mip::Value const optimum_value_bmccf = solution_bmccf->optimum_value();
		std::cout << "optimum value BMCCF             = " << optimum_value_bmccf << "\n";
	}

	if (solution_emcf_integer.has_value()) {
		mip::Value const optimum_value_emcf_integer = solution_emcf_integer->optimum_value();
		std::cout << "optimum value EMCF integer      = " << optimum_value_emcf_integer << "\n";
		CRITICAL_CHECK(
			optimum_value_emcf_integer == optimum_value_dijkstra_steiner,
			"EMFC (MIP) and Dijkstra Steiner should have the same optimum value, error = "
			+ std::to_string(std::abs(optimum_value_emcf_integer - optimum_value_dijkstra_steiner))
		)
	}

//	write_json(std::cout, analysis);

	return analysis;
}
