#include "steiner_trees.hpp"

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
	Logger::set_logger(std::make_shared<std::ofstream>(instance_path + "/" + instance_name + ".log"));

	Logger::logger() << "Steiner Tree LP/MIP Solver\n"
					 << "\n"
					 << "program path  : " << program_path << "\n"
					 << "instance path : " << instance_path << "\n"
					 << "instance name : " << instance_name << "\n"
					 << "\n";

	json const instance_json = read_json(instance_path + "/" + instance_name + ".json");

	Logger::logger() << instance_path << "/" << instance_name << ".json\n"
					 << instance_json.dump(4)
					 << "\n\n";

	graph::Graph const graph = graph::GraphFactory::create_from_json(instance_json["graph"]);
	graph::Net::Vector const nets = graph::NetsFactory::create_from_json(instance_json["nets"]);

	assert(nets.size() == 1);

	graph::GraphPrinter::output(Logger::logger(), graph);

	graph::TerminalInstance const terminal_instance(graph, nets.front().terminals());

	steiner_trees::SteinerTreeProblem const steiner_tree_problem("test", terminal_instance, graph, nets);

	steiner_trees::SteinerTreeSolution const solution_continuous = steiner_trees::SteinerTreeSolver::solve(
		steiner_tree_problem, steiner_trees::SteinerTreeMIP::EMC, mip::MIP::LINEAR_PROGRAMMING
	);

	steiner_trees::SteinerTreeSolution const solution_simplex_embedding = steiner_trees::SteinerTreeSolver::solve(
		steiner_tree_problem, steiner_trees::SteinerTreeMIP::SIMPLEX_EMBEDDING, mip::MIP::LINEAR_PROGRAMMING
	);

	steiner_trees::SteinerTreeSolution const solution_bmccf = steiner_trees::SteinerTreeSolver::solve(
		steiner_tree_problem,
		steiner_trees::SteinerTreeMIP::BIDIRECTED_MULTI_COMMODITY_COMMON_FLOW,
		mip::MIP::LINEAR_PROGRAMMING
	);

	if (solution_continuous.optimization_result() == mip::MIP::Solver::INFEASIBLE) {
		std::cout << "LP is infeasible!\n";
	} else {
		assert(solution_continuous.optimization_result() == mip::MIP::Solver::OPTIMAL);
		assert(solution_simplex_embedding.optimization_result() == mip::MIP::Solver::OPTIMAL);
		assert(solution_bmccf.optimization_result() == mip::MIP::Solver::OPTIMAL);

		std::cout << "optimum value continuous        = "
				  << solution_continuous.optimization_value()
				  << "\n";
		std::cout << "optimum value simplex embedding = "
				  << solution_simplex_embedding.optimization_value()
				  << "\n";
		std::cout << "optimum value BMCCF             = "
				  << solution_bmccf.optimization_value()
				  << "\n";

//		assert(solution_continuous.optimization_value() == solution_simplex_embedding.optimization_value());
		assert(std::abs(solution_continuous.optimization_value() - solution_simplex_embedding.optimization_value()) < 1e-9);
	}

	steiner_trees::SteinerTreeSolution const solution_integer = steiner_trees::SteinerTreeSolver::solve(
		steiner_tree_problem, steiner_trees::SteinerTreeMIP::EMC, mip::MIP::MIXED_INTEGER_PROGRAMMING
	);

	if (solution_integer.optimization_result() == mip::MIP::Solver::INFEASIBLE) {
		std::cout << "MIP is infeasible!\n";
	}

	if (solution_continuous.optimization_result() == mip::MIP::Solver::OPTIMAL
		and solution_simplex_embedding.optimization_result() == mip::MIP::Solver::OPTIMAL
		and solution_integer.optimization_result() == mip::MIP::Solver::OPTIMAL) {

		mip::Value const optimum_value_continuous = solution_continuous.optimization_value();
		mip::Value const optimum_value_simplex_embedding = solution_simplex_embedding.optimization_value();
		mip::Value const optimum_value_bmccf = solution_bmccf.optimization_value();
		mip::Value const optimum_value_integer = solution_integer.optimization_value();
		mip::Value const integrality_gap = optimum_value_integer / optimum_value_continuous;

//		assert(optimum_value_continuous == optimum_value_simplex_embedding);
		assert(std::abs(optimum_value_continuous - optimum_value_simplex_embedding) < 1e-9);

//		assert(optimum_value_bmccf == optimum_value_integer);
		assert(std::abs(optimum_value_bmccf - optimum_value_integer) < 1e-9);

		std::cout << "\n"
				  << "optimum value continuous        = " << optimum_value_continuous << "\n"
				  << "optimum value simplex embedding = " << optimum_value_simplex_embedding << "\n"
				  << "optimum value BMCCF             = " << optimum_value_bmccf << "\n"
				  << "optimum value integer           = " << optimum_value_integer << "\n"
				  << "integrality gap                 = " << integrality_gap << "\n"
				  << "\n";

		json const output_json = {
//			{
//				"problem",
//				steiner_tree_problem.export_to_json()
//			},
			{
				"solution",
				{
					{"continuous", solution_continuous.export_to_json()},
					{"simplex_embedding", solution_simplex_embedding.export_to_json()},
					{"bmccf", solution_bmccf.export_to_json()},
					{"integer", solution_integer.export_to_json()}
				}
			}
		};

		std::string const instance_file_path = instance_path + "/" + instance_name + ".json";
		std::string const solution_file_path = instance_path + "/" + "solution_" + instance_name + ".json";

		Logger::logger() << solution_file_path << "\n"
						 << output_json.dump(4)
						 << "\n\n";

		write_json(solution_file_path, output_json);

		std::cout << solution_file_path << "\n";

		int const status = system(("python3 /home/robert/Documents/studies/discrete_mathematics/graph-plotter/simplex_embedding.py " + instance_file_path + " " + solution_file_path + " " + instance_path).c_str());

		if (not status) {

		}
	}
}