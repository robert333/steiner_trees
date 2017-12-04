#include <cassert>
#include <string>
#include <iostream>

#include "steiner_trees.hpp"
#include "yannick.hpp"

int main(int argc, char* argv[])
{
	assert(argc > 0);

	if (argc < 3) {
		std::cerr << "ERROR: missing parameters\n"
				  << argv[0] << " instance_path instance_name\n";
		return 1;
	}

	std::string const program_path = argv[0];
	std::string const instance_path = argv[1];
	std::string const instance_name = argv[2];

	run_steiner_trees(program_path, instance_path, instance_name);
//	run_yannick(program_path, instance_path, instance_name);

	return 0;
}