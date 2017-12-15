#include "SkutellaProblem.hpp"

namespace skutella {

SkutellaProblem::SkutellaProblem(
	std::string const& name,
	Number num_terminals,
	Number num_paths,
	Number num_integral_trees
) :
	_name(name),
	_num_terminals(num_terminals),
	_num_paths(num_paths),
	_num_integral_trees(num_integral_trees)
{}

std::string const& SkutellaProblem::name() const
{
	return _name;
}

skutella::Number SkutellaProblem::num_terminals() const
{
	return _num_terminals;
}

skutella::Number SkutellaProblem::num_paths() const
{
	return _num_paths;
}

skutella::Number SkutellaProblem::num_integral_trees() const
{
	return _num_integral_trees;
}

json SkutellaProblem::export_to_json() const
{
	json result;

	result["num_terminals"] = num_terminals();
	result["num_paths"] = num_paths();
	result["num_integral_trees"] = num_integral_trees();

	return result;
}

} // namespace skutella
