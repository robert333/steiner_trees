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
	yannick::Index const min_cycle_number = instance_json["min_cycle_number"];
	yannick::Index const max_cycle_number = instance_json["max_cycle_number"];
	yannick::Index const min_machine_number = instance_json["min_machine_number"];
	yannick::Index const max_machine_number = instance_json["max_machine_number"];
	yannick::Index const min_jumper_number = instance_json["min_jumper_number"];
	yannick::Index const max_jumper_number = instance_json["max_jumper_number"];

	yannick::YannickProblem const yannick_problem(
		"yannick",
		precedence_graph,
		cycle_time,
		min_cycle_number,
		max_cycle_number,
		min_machine_number,
		max_machine_number,
		min_jumper_number,
		max_jumper_number
	);

	graph::GraphPrinter::output(Logger::logger(), yannick_problem.precedence_graph());

	Logger::logger() << "\n"
					 << "cycle time         : " << yannick_problem.cycle_time() << "\n"
					 << "cycle number       : " << yannick_problem.cycle_number() << "\n"
					 << "min cycle number : " << yannick_problem.min_cycle_number() << "\n"
					 << "max cycle number : " << yannick_problem.max_cycle_number() << "\n"
					 << "min machine number : " << yannick_problem.min_machine_number() << "\n"
					 << "max machine number : " << yannick_problem.max_machine_number() << "\n"
					 << "min jumper number  : " << yannick_problem.min_jumper_number() << "\n"
					 << "max jumper number  : " << yannick_problem.max_jumper_number() << "\n"
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

//		Logger::logger() << instance_path << "/" << "solution_" << instance_name << ".json\n"
//						 << output_json.dump(4)
//						 << "\n\n";

		write_json(instance_path + "/" + instance_name + "_solution" + ".json", output_json);
	} else {
		assert(false);
	}
}