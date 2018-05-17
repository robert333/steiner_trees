#include "Chronograph.hpp"

#include <iostream>

Chronograph::Chronograph(std::string const& name) :
	_name(name),
	_start_time_point(std::chrono::high_resolution_clock::now())
{}

Chronograph::~Chronograph()
{
	auto const duration = std::chrono::high_resolution_clock::duration(
		std::chrono::high_resolution_clock::now() - _start_time_point
	);

	std::cout << "[Chronograph] " << _name << " : "
			  << std::chrono::duration_cast<std::chrono::minutes>(duration).count() << " min "
			  << std::chrono::duration_cast<std::chrono::seconds>(duration % std::chrono::minutes(1)).count() << " sec"
			  << std::endl;
}
