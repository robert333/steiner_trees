#include "graph/GraphFactory.hpp"
#include "graph/GraphPrinter.hpp"
#include "graph/Net.hpp"
#include "graph/NetsFactory.hpp"

#include "steiner_trees/SteinerTreeProblem.hpp"
#include "steiner_trees/SteinerTreeSolver.hpp"

#include "Logger.hpp"

int main(int argc, char* argv[])
{
	assert(argc > 0);

	if (argc < 3) {
		std::cerr << "ERROR: missing parameters\n"
				  << argv[0] << " input_file_path output_file_path\n";
		return 0;
	}

	std::string const program_path = argv[0];
	std::string const instance_path = argv[1];
	std::string const instance_name = argv[2];

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

	graph::Graph const graph = graph::GraphFactory::create_from_json(instance_json["graph"]);//.bidirect();
	graph::Net::Vector const nets = graph::NetsFactory::create_from_json(instance_json["nets"]);

	assert(nets.size() == 1);

	graph::GraphPrinter::output(Logger::logger(), graph);

	steiner_trees::SteinerTreeProblem const steiner_tree_problem("test", graph, nets);

	steiner_trees::SteinerTreeSolution const steiner_tree_solution_continuous = steiner_trees::SteinerTreeSolver::solve(
		steiner_tree_problem, mip::MIP::LINEAR_PROGRAMMING
	);

	if (steiner_tree_solution_continuous.optimization_result() == mip::MIP::Solver::INFEASIBLE) {
		std::cout << "LP is infeasible!\n";
	} else {
		std::cout << "optimization_value_continuous = "
				  << steiner_tree_solution_continuous.optimization_value()
				  << "\n";
	}

	steiner_trees::SteinerTreeSolution const steiner_tree_solution_integer = steiner_trees::SteinerTreeSolver::solve(
		steiner_tree_problem, mip::MIP::MIXED_INTEGER_PROGRAMMING
	);

	if (steiner_tree_solution_integer.optimization_result() == mip::MIP::Solver::INFEASIBLE) {
		std::cout << "MIP is infeasible!\n";
	}

	if (steiner_tree_solution_continuous.optimization_result() == mip::MIP::Solver::OPTIMAL
		and steiner_tree_solution_integer.optimization_result() == mip::MIP::Solver::OPTIMAL) {
		mip::MIP::Value const optimization_value_integer = steiner_tree_solution_integer.optimization_value();
		mip::MIP::Value const optimization_value_continuous = steiner_tree_solution_continuous.optimization_value();
		mip::MIP::Value const integrality_gap = optimization_value_integer / optimization_value_continuous;
		std::cout << "\n"
				  << "optimization_value_integer    = " << optimization_value_integer << "\n"
				  << "optimization_value_continuous = " << optimization_value_continuous << "\n"
				  << "Integrality Gap = " << integrality_gap << "\n\n";

		json const output_json = {
			{
				"problem",
				steiner_tree_problem.export_to_json()
			},
			{
				"solution",
				{
					{"integer", steiner_tree_solution_integer.export_to_json()},
					{"continuous", steiner_tree_solution_continuous.export_to_json()}
				}
			}
		};

		Logger::logger() << instance_path << "/" << "solution_" << instance_name << ".json\n"
						 << output_json.dump(4)
						 << "\n\n";

		write_json(instance_path + "/" + "solution_" + instance_name + ".json", output_json);
	}

	return 0;
}