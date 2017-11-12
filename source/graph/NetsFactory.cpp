#include "NetsFactory.hpp"

namespace graph {

std::vector<Net> NetsFactory::create_from_json(json const& nets_json)
{
	std::vector<Net> nets;

	for (json const& net_json : nets_json) {
		std::vector<NodeId> terminals;

		for (json const& terminal_json : net_json["terminals"]) {
			terminals.push_back(terminal_json.get<NodeId>());
		}

		nets.emplace_back(nets.size(), net_json["name"].get<std::string>(), terminals);
	}

	return nets;
}

} // namespace graph
