#ifndef SKUTELLA_SKUTELLAPROBLEM_HPP
#define SKUTELLA_SKUTELLAPROBLEM_HPP

#include <string>
#include <vector>
#include <memory>

#include "../json.hpp"
#include "../typedefs.hpp"

namespace skutella {

class SkutellaProblem {
public:
	using SharedPtr = std::shared_ptr<SkutellaProblem>;
	using Vector = std::vector<SkutellaProblem>;

public:
	explicit SkutellaProblem(
		std::string const& name,
		Quantity num_terminals,
		Quantity num_paths,
		Quantity num_integral_trees
	);

	std::string const& name() const;
	Quantity num_terminals() const;
	Quantity num_paths() const;
	Quantity num_integral_trees() const;

	json export_to_json() const;

private:

private:
	std::string _name;
	Quantity _num_terminals;
	Quantity _num_paths;
	Quantity _num_integral_trees;
};

} // namespace skutella

#endif
