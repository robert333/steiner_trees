#include "GroupSkutella.hpp"

#include "../Logger.hpp"

namespace skutella {

GroupSkutella::GroupSkutella(SkutellaProblem const& skutella_problem) :
	Group("GroupSkutella"),
	_skutella_problem(skutella_problem)
{}

void GroupSkutella::create_variables_constraints_and_objective(mip::MIPModel& mip_model)
{
	create_variables(mip_model);
	create_constraints(mip_model);
	create_objective(mip_model);
}

json GroupSkutella::compute_solution() const
{
	json solution;

	for (Index set = 0; set < _skutella_problem.num_terminals(); ++set) {
		std::vector<Number> terminals;

		for (Index terminal = 0; terminal < _skutella_problem.num_terminals(); ++terminal) {
			if (_set_variables.solution_value(set, terminal) == 1) {
				terminals.push_back(terminal);
			}
		}

		solution["sets"].push_back({{"set", set}, {"terminals", terminals}});
	}

	helper::PowerSetIterator power_set_iterator(
		_skutella_problem.num_terminals(), _skutella_problem.num_integral_trees() - 1
	);

	do {
		helper::PowerSetIterator::Set const set_cover = power_set_iterator.compute_current_subset();

		std::vector<Index> terminals;

		for (Index terminal = 0; terminal < _skutella_problem.num_terminals(); ++terminal) {
			if (_set_cover_variables.solution_value(set_cover, terminal) == 1) {
				terminals.push_back(terminal);
			}
		}

		solution["set_covers"].push_back(
			{
				{"set_cover", power_set_iterator.compute_current_subset()},
				{"terminals", terminals}
			}
		);
	} while (power_set_iterator.next());

	solution["paths"] = _path_variable.solution_value(0);

	return solution;
}

void GroupSkutella::create_variables(mip::MIPModel& mip_model)
{
	for (Index set = 0; set < _skutella_problem.num_terminals(); ++set) {
		for (Index terminal = 0; terminal < _skutella_problem.num_terminals(); ++terminal) {
			mip::MIPModel::Variable* const variable = mip_model.create_binary_variable(
				name(), "set " + std::to_string(set) + ", terminal " + std::to_string(terminal)
			);
			_set_variables.set(set, terminal, variable);
		}
	}

	helper::PowerSetIterator power_set_iterator(
		_skutella_problem.num_terminals(), _skutella_problem.num_integral_trees() - 1
	);

	do {
		helper::PowerSetIterator::Set const set_cover = power_set_iterator.compute_current_subset();

		Logger::logger() << "set_cover " + helper::to_string(set_cover) << "\n";

		for (Index terminal = 0; terminal < _skutella_problem.num_terminals(); ++terminal) {
			mip::MIPModel::Variable* const variable = mip_model.create_binary_variable(
				name(), "set_cover " + helper::to_string(set_cover) + ", terminal " + std::to_string(terminal)
			);

			_set_cover_variables.set(set_cover, terminal, variable);
		}
	} while (power_set_iterator.next());

	mip::MIPModel::Variable* const path_variable = mip_model.create_integer_variable(
		name(), "path_variable", _skutella_problem.num_paths(), _skutella_problem.num_paths()//0, _skutella_problem.num_terminals()
	);

	_path_variable.set(0, path_variable);
}

void GroupSkutella::create_constraints(mip::MIPModel& mip_model)
{
	for (Index set = 0; set < _skutella_problem.num_terminals(); ++set) {
		mip::Constraint constraint = mip_model.create_constraint(
			name(), "cardinality for set " + std::to_string(set)
		);

		constraint.add_variable(_path_variable.get(0), -1);

		for (Index terminal = 0; terminal < _skutella_problem.num_terminals(); ++terminal) {
			constraint.add_variable(_set_variables.get(set, terminal), 1);
		}

		constraint.set_lower_bound(0);
		constraint.set_upper_bound(0);
	}

	for (Index terminal = 0; terminal < _skutella_problem.num_terminals(); ++terminal) {
		mip::Constraint constraint = mip_model.create_constraint(
			name(), "access for terminal " + std::to_string(terminal)
		);

		constraint.add_variable(_path_variable.get(0), -1);

		for (std::size_t set = 0; set < _skutella_problem.num_terminals(); ++set) {
			constraint.add_variable(_set_variables.get(set, terminal), 1);
		}

		constraint.set_lower_bound(0);
		constraint.set_upper_bound(0);
	}

	helper::PowerSetIterator power_set_iterator(
		_skutella_problem.num_terminals(), _skutella_problem.num_integral_trees() - 1
	);

	do {
		helper::PowerSetIterator::Set const set_cover = power_set_iterator.compute_current_subset();

		mip::Constraint constraint_set_cover = mip_model.create_constraint(
			name(), "set_cover " + helper::to_string(set_cover) + " should not cover all terminals"
		);

		constraint_set_cover.set_upper_bound(_skutella_problem.num_terminals() - 1);

		for (Index terminal = 0; terminal < _skutella_problem.num_terminals(); ++terminal) {
			constraint_set_cover.add_variable(_set_cover_variables.get(set_cover, terminal), 1);

//			mip::Constraint constraint_terminal_cover = mip_model.create_constraint(
//				name(),
//				"is terminal " + std::to_string(terminal)
//				+ " covered by set_cover " + helper::to_string(set_cover)
//			);
//
//			constraint_terminal_cover.add_variable(_set_cover_variables.get(set_cover, terminal), -1);
//			constraint_terminal_cover.set_upper_bound(0);
//
//			double const factor = 1.0 / (2.0 * static_cast<double>(set_cover.size()));

			for (Index set : set_cover) {
//				constraint_terminal_cover.add_variable(_set_variables.get(set, terminal), factor);

				mip::Constraint constraint = mip_model.create_constraint(
					name(),
					"is terminal " + std::to_string(terminal)
					+ " covered within set_cover " + helper::to_string(set_cover)
					+ " by set " + std::to_string(set)
				);

				constraint.add_variable(_set_cover_variables.get(set_cover, terminal), -1);
				constraint.add_variable(_set_variables.get(set, terminal), 1);
				constraint.set_upper_bound(0);
			}
		}
	} while (power_set_iterator.next());
}

void GroupSkutella::create_objective(mip::MIPModel& mip_model)
{
	mip_model.set_objective_coefficient(_path_variable.get(0), 1);
}

} // namespace skutella
