#include "graph/GraphFactory.hpp"
#include "graph/GraphPrinter.hpp"

#include "steiner_trees/SteinerTreeProblem.hpp"
#include "steiner_trees/SteinerTreeSolver.hpp"

#include "Logger.hpp"

#include "yannick/YannickProblem.hpp"
#include "yannick/YannickSolution.hpp"
#include "yannick/YannickSolver.hpp"

void run_yannick(
	std::string const& program_path,
	std::string const& instance_path,
	std::string const& instance_name
)
{
	Logger::set_logger(std::make_shared<std::ofstream>(instance_path + "/" + instance_name + ".log"));

	Logger::logger() << "Yannick LP/MIP Solver\n"
					 << "\n"
					 << "program path  : " << program_path << "\n"
					 << "instance path : " << instance_path << "\n"
					 << "instance name : " << instance_name << "\n"
					 << "\n";

	json const instance_json = read_json(instance_path + "/" + instance_name + ".json");

	Logger::logger() << instance_path << "/" << instance_name << ".json\n"
					 << instance_json.dump(4)
					 << "\n\n";

	graph::Graph const precedence_graph = graph::GraphFactory::create_from_json(instance_json["precedence_graph"]);
	double const cycle_time = instance_json["cycle_time"];
	int const cycle_number = instance_json["cycle_number"];
	int const machine_number = instance_json["machine_number"];

	yannick::YannickProblem const yannick_problem(
		"yannick",
		precedence_graph,
		cycle_time,
		cycle_number,
		machine_number
	);

	graph::GraphPrinter::output(Logger::logger(), yannick_problem.precedence_graph());

	Logger::logger() << "\n"
					 << "cycle time     : " << cycle_time << "\n"
					 << "cycle number   : " << cycle_number << "\n"
					 << "machine number : " << machine_number << "\n"
					 << "\n";

	yannick::YannickSolution const yannick_solution = yannick::YannickSolver::solve(yannick_problem);

	if (yannick_solution.optimization_result() == mip::MIP::Solver::INFEASIBLE) {
		std::cout << "MIP is infeasible!\n";
	} else if (yannick_solution.optimization_result() == mip::MIP::Solver::OPTIMAL) {
		std::cout << "MIP solved optimally!\n"
				  << "optimization_value = " << yannick_solution.optimization_value() << "\n\n";

		json const output_json = {
			{"problem", yannick_problem.export_to_json()},
			{"solution", yannick_solution.export_to_json()}
		};

		Logger::logger() << instance_path << "/" << "solution_" << instance_name << ".json\n"
						 << output_json.dump(4)
						 << "\n\n";

		write_json(instance_path + "/" + "solution_" + instance_name + ".json", output_json);
	} else {
		assert(false);
	}
}