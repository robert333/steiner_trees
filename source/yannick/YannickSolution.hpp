#ifndef YANNICK_YANNICKSOLUTION_HPP
#define YANNICK_YANNICKSOLUTION_HPP

#include "../mip/Solution.hpp"

namespace yannick {

class YannickSolution : public mip::Solution {
public:
	using SharedPtr = std::shared_ptr<YannickSolution>;
	using Vector = std::vector<YannickSolution>;

public:
	explicit YannickSolution(mip::Solution const& mip_solution);

private:

private:
};

} // namespace yannick

#endif
