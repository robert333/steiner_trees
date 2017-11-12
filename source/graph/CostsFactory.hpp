#ifndef GRAPH_COSTSFACTORY_HPP
#define GRAPH_COSTSFACTORY_HPP

#include "Costs.hpp"
#include "../json.hpp"

namespace graph {

class CostsFactory {
public:

public:
	static Costs create_from_json(json const& costs_json);

private:
	explicit CostsFactory() = default;

private:

};

} // namespace graph

#endif
