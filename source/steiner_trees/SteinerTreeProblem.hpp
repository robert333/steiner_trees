#ifndef STEINER_TREES_STEINERTREEPROBLEM_HPP
#define STEINER_TREES_STEINERTREEPROBLEM_HPP

#include "../graph/Graph.hpp"
#include "../graph/Net.hpp"
#include "../mip/MIP.hpp"

namespace steiner_trees {

class SteinerTreeProblem {
public:
	using SharedPtr = std::shared_ptr<SteinerTreeProblem>;
	using Vector = std::vector<SteinerTreeProblem>;

public:
	explicit SteinerTreeProblem(
		std::string const& name,
		graph::Graph const& graph,
		graph::Net::Vector const& nets
	);

	std::string const& name() const;
	graph::Graph const& graph() const;
	graph::Net::Vector const& nets() const;

	json export_to_json() const;

private:

private:
	std::string _name;
	graph::Graph _graph;
	graph::Net::Vector _nets;
};

} // namespace steiner_trees

#endif
