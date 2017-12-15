#ifndef SKUTELLA_SKUTELLASOLUTION_HPP
#define SKUTELLA_SKUTELLASOLUTION_HPP

#include "../mip/Solution.hpp"

namespace skutella {

class SkutellaSolution : public mip::Solution {
public:
	using SharedPtr = std::shared_ptr<SkutellaSolution>;
	using Vector = std::vector<SkutellaSolution>;

public:
	explicit SkutellaSolution(mip::Solution const& mip_solution);

private:

private:
};

} // namespace skutella

#endif
