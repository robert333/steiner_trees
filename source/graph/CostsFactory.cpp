#include "CostsFactory.hpp"

namespace graph {

Costs CostsFactory::create_from_json(json const& /*costs_json*/)
{
	return Costs();
}

} // namespace graph
