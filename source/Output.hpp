#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <string>
#include <ostream>

class Output {
public:
	virtual std::string to_string() const = 0;

protected:
	explicit Output() = default;
};

std::ostream& operator<<(std::ostream& ostream, Output const& output);

#endif
