#ifndef MIP_SOLUTION_HPP
#define MIP_SOLUTION_HPP

#include "../json.hpp"

namespace mip {

class Solution {
public:
	explicit Solution() = default;

	void set(std::string const& group_name, json const& group_solution);
	json get(std::string const& group_name) const;

private:
	std::map<std::string, json> _solutions;
};

} // namespace mip

#endif
