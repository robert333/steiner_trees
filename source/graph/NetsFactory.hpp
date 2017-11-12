#ifndef GRAPH_NETSFACTORY_HPP
#define GRAPH_NETSFACTORY_HPP

#include "Net.hpp"
#include "../json.hpp"

namespace graph {

class NetsFactory {
public:

public:
	static Net::Vector create_from_json(json const& nets_json);

private:
	explicit NetsFactory() = default;

private:

};

} // namespace graph

#endif
