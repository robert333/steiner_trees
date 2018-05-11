#include "steiner_trees.hpp"

#include <fstream>

#include "graph/GraphFactory.hpp"
#include "graph/GraphPrinter.hpp"
#include "graph/Net.hpp"
#include "graph/NetsFactory.hpp"

#include "steiner_trees/SteinerTreeProblem.hpp"
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

	json const instance_json = read_json(instance_path + "/" + instance_name + ".json");

	graph::Graph const graph = graph::GraphFactory::create_from_json(instance_json["graph"]);
	graph::Net::Vector const nets = graph::NetsFactory::create_from_json(instance_json["nets"]);

	assert(nets.size() == 1);

	graph::TerminalInstance const terminal_instance(graph, nets.front().terminals());

	steiner_trees::SteinerTreeProblem const steiner_tree_problem("test", terminal_instance, graph, nets);

	analyze_bidirected_cut_relaxation(steiner_tree_problem);
}

void analyze_bidirected_cut_relaxation(steiner_trees::SteinerTreeProblem const& steiner_tree_problem)
{
	auto const solution_emcf_continuous = steiner_trees::SteinerTreeSolver::solve_via_linear_programming(
		steiner_tree_problem,
		steiner_trees::SteinerTreeMIP::EMCF,
		mip::MIP::LINEAR_PROGRAMMING
	);

	auto const solution_emcf_integer = steiner_trees::SteinerTreeSolver::solve_via_linear_programming(
		steiner_tree_problem,
		steiner_trees::SteinerTreeMIP::EMCF,
		mip::MIP::MIXED_INTEGER_PROGRAMMING
	);
//
//	auto const solution_simplex_embedding = steiner_trees::SteinerTreeSolver::solve_via_linear_programming(
//		steiner_tree_problem,
//		steiner_trees::SteinerTreeMIP::SIMPLEX_EMBEDDING,
//		mip::MIP::LINEAR_PROGRAMMING
//	);
//
//	auto const solution_bmccf = steiner_trees::SteinerTreeSolver::solve_via_linear_programming(
//		steiner_tree_problem,
//		steiner_trees::SteinerTreeMIP::BIDIRECTED_MULTI_COMMODITY_COMMON_FLOW,
//		mip::MIP::MIXED_INTEGER_PROGRAMMING
//	);

	auto const solution_dijkstra_steiner = steiner_trees::SteinerTreeSolver::solve_via_combinatorial_optimization(
		steiner_tree_problem
	);

	mip::Value const optimum_value_emcf_continuous = solution_emcf_continuous.optimum_value();
	mip::Value const optimum_value_emcf_integer = solution_emcf_integer.optimum_value();
//	mip::Value const optimum_value_simplex_embedding = solution_simplex_embedding.optimum_value();
//	mip::Value const optimum_value_bmccf = solution_bmccf.optimum_value();
	mip::Value const optimum_value_dijkstra_steiner = solution_dijkstra_steiner.optimum_value();
	mip::Value const integrality_gap = optimum_value_dijkstra_steiner / optimum_value_emcf_continuous;

	std::cout << "optimum value EMCF continuous   = " << optimum_value_emcf_continuous << "\n";
	std::cout << "optimum value EMCF integer      = " << optimum_value_emcf_integer << "\n";
//	std::cout << "optimum value simplex embedding = " << optimum_value_simplex_embedding << "\n";
//	std::cout << "optimum value BMCCF             = " << optimum_value_bmccf << "\n";
	std::cout << "optimum value Dijkstra Steiner  = " << optimum_value_dijkstra_steiner << "\n";
	std::cout << "integrality gap EMCF            = " << integrality_gap << "\n";

	write_json(std::cout, solution_dijkstra_steiner.solution());

	assert(optimum_value_emcf_integer == optimum_value_dijkstra_steiner);

//	assert(solution_continuous.optimum_value() == solution_simplex_embedding.optimum_value());
//	std::cerr << std::abs(solution_emcf_continuous.optimum_value() - solution_simplex_embedding.optimum_value()) << "\n";
//	assert(std::abs(solution_continuous.optimum_value() - solution_simplex_embedding.optimum_value()) < 1e-9);

//	assert(optimum_value_emcf_continuous == optimum_value_simplex_embedding);
//	std::cerr << std::abs(optimum_value_emcf_continuous - optimum_value_simplex_embedding) << "\n";
//	assert(std::abs(optimum_value_emcf_continuous - optimum_value_simplex_embedding) < 1e-9);

//	assert(optimum_value_bmccf == optimum_value_emcf_integer);
//	assert(std::abs(optimum_value_bmccf - optimum_value_emcf_integer) < 1e-9);

//	json const solution_json = {
//		{"edges", solution_emc.export_to_json()["variables"]["GroupEdges"]["activated"]}
//	};
//
//	write_json(instance_path + "/solution.json", solution_json);
//
//	json const output_json = {
////		{
////			"problem",
////			steiner_tree_problem.export_to_json()
////		},
//		{
//			"solution",
//			{
//				{"continuous", solution_continuous.export_to_json()},
//				{"simplex_embedding", solution_simplex_embedding.export_to_json()},
////				{"bmccf", solution_bmccf.export_to_json()},
//				{"integer", solution_emc.export_to_json()}
//			}
//		}
//	};
//
//	std::string const instance_file_path = instance_path + "/" + instance_name + ".json";
//	std::string const solution_file_path = instance_path + "/" + "solution_" + instance_name + ".json";
//
//	write_json(solution_file_path, output_json);
//
//	std::cout << solution_file_path << "\n";
//
//	int const status = system(("python3 /home/robert/Documents/studies/discrete_mathematics/graph-plotter/simplex_embedding.py " + instance_file_path + " " + solution_file_path + " " + instance_path).c_str());
//
//	if (not status) {
//
//	}
}
