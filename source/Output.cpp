#include "Output.hpp"

std::ostream& operator<<(std::ostream& ostream, Output const& output)
{
	return ostream << output.to_string();
}
