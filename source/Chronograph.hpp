#ifndef CHRONOGRAPH_HPP
#define CHRONOGRAPH_HPP

#include <chrono>
#include <string>

class Chronograph {
public:
	explicit Chronograph(std::string const& name);
	~Chronograph();

private:
	std::string _name;
	std::chrono::high_resolution_clock::time_point _start_time_point;
};

#endif
