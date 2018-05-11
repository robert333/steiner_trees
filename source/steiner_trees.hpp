#ifndef STEINER_TREES_HPP
#define STEINER_TREES_HPP

#include <steiner_trees/SteinerTreeProblem.hpp>

void run_steiner_trees(
	std::string const& program_path,
	std::string const& instance_path,
	std::string const& instance_name
);

void analyze_bidirected_cut_relaxation(steiner_trees::SteinerTreeProblem const& steiner_tree_problem);

#endif