#ifndef STEINER_TREES_STEINERTREESOLUTION_HPP
#define STEINER_TREES_STEINERTREESOLUTION_HPP

#include "../json.hpp"
#include "../graph/typedefs.hpp"
#include "../mip/Solution.hpp"

namespace steiner_trees {

class SteinerTreeSolution : public mip::Solution {
public:
	using SharedPtr = std::shared_ptr<SteinerTreeSolution>;
	using Vector = std::vector<SteinerTreeSolution>;

public:
	explicit SteinerTreeSolution(mip::Solution const& mip_solution);

private:

private:
};

} // namespace steiner_trees

#endif
