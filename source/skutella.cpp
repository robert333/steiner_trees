#include "skutella.hpp"

#include <fstream>

#include "Logger.hpp"

#include "skutella/SkutellaProblem.hpp"
#include "skutella/SkutellaSolution.hpp"
#include "skutella/SkutellaSolver.hpp"

void run_skutella(
	std::string const& program_path,
	std::string const& instance_path,
	std::string const& instance_name
)
{
	std::ofstream log(instance_path + "/" + instance_name + ".log");

	Logger::set_logger(log);

	Logger::logger() << "Skutella LP/MIP Solver\n"
					 << "\n"
					 << "program path  : " << program_path << "\n"
					 << "instance path : " << instance_path << "\n"
					 << "instance name : " << instance_name << "\n"
					 << "\n";

	json const instance_json = read_json(instance_path + "/" + instance_name + ".json");

	Logger::logger() << instance_path << "/" << instance_name << ".json\n"
					 << instance_json.dump(4)
					 << "\n\n";

	Number const num_terminals = instance_json["num_terminals"];
	Number const num_paths = instance_json["num_paths"];
	Number const num_integral_trees = instance_json["num_integral_trees"];

	skutella::SkutellaProblem const skutella_problem(
		"skutella",
		num_terminals,
		num_paths,
		num_integral_trees
	);

	Logger::logger() << "\n"
					 << "num_terminals      : " << skutella_problem.num_terminals() << "\n"
					 << "num_paths          : " << skutella_problem.num_paths() << "\n"
					 << "num_integral_trees : " << skutella_problem.num_integral_trees() << "\n"
					 << "\n";

	skutella::SkutellaSolution const skutella_solution = skutella::SkutellaSolver::solve(skutella_problem);

	if (skutella_solution.optimization_result() == mip::MIP::Solver::INFEASIBLE) {
		std::cout << "MIP is infeasible!\n";
	} else if (skutella_solution.optimization_result() == mip::MIP::Solver::OPTIMAL) {
		std::cout << "MIP solved optimally!\n"
				  << "optimization_value = " << skutella_solution.optimization_value() << "\n\n";

		double const t = skutella_problem.num_terminals();
		double const p = skutella_solution.optimization_solution()["skutella"]["paths"];
		double const d = skutella_problem.num_integral_trees();

		double const integrality_gap = p * (t + d) / ((p + 1) * t);

		std::cout << "integrality gap = " << integrality_gap << "\n";

		json const output_json = {
			{"problem", skutella_problem.export_to_json()},
			{"solution", skutella_solution.export_to_json()}
		};

		Logger::logger() << instance_path << "/" << "solution_" << instance_name << ".json\n"
						 << output_json.dump(4)
						 << "\n\n";

		write_json(instance_path + "/" + instance_name + "_solution" + ".json", output_json);
	} else {
		assert(false);
	}
}