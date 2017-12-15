#ifndef SKUTELLA_SKUTELLAPROBLEM_HPP
#define SKUTELLA_SKUTELLAPROBLEM_HPP

#include <string>
#include <vector>
#include <memory>

#include "../json.hpp"

namespace skutella {

using Number = int;

class SkutellaProblem {
public:
	using SharedPtr = std::shared_ptr<SkutellaProblem>;
	using Vector = std::vector<SkutellaProblem>;

public:
	explicit SkutellaProblem(
		std::string const& name,
		Number num_terminals,
		Number num_paths,
		Number num_integral_trees
	);

	std::string const& name() const;
	Number num_terminals() const;
	Number num_paths() const;
	Number num_integral_trees() const;

	json export_to_json() const;

private:

private:
	std::string _name;
	Number _num_terminals;
	Number _num_paths;
	Number _num_integral_trees;
};

} // namespace skutella

#endif
