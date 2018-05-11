#ifndef STEINERTREES_DIJKSTRASTEINER_STEINERTREELABELTABLE_HPP
#define STEINERTREES_DIJKSTRASTEINER_STEINERTREELABELTABLE_HPP

#include <data/BitSetTable.hpp>

#include <steiner_trees/dijkstra_steiner/SteinerTreeLabel.hpp>

namespace steiner_trees {
namespace dijkstra_steiner {

/**
 * Class to store Label informations like the current cost or the backtrack edges
 */
template<std::size_t num_terminals>
class SteinerTreeLabelTable {
public:
	using Key = SteinerTreeKey<num_terminals>;
	using Label = SteinerTreeLabel<num_terminals>;
	using Cost = typename Label::Cost;
	using Backtrack = typename Label::Backtrack;

	struct LabelInformation {
		explicit LabelInformation(
			bool const initialised = false,
			Cost const minimum_cost = std::numeric_limits<Cost>::max(),
			Backtrack const& backtrack = std::make_pair<Key, Key>(Key::invalid(), Key::invalid())
		) :
			initialised(initialised),
			minimum_cost(minimum_cost),
			backtrack(backtrack)
		{}

		bool initialised;
		Cost minimum_cost;
		Backtrack backtrack;
	};

public:
	bool is_processed(SteinerTreeKey<num_terminals> const& steiner_tree_key) const
	{
		return steiner_tree_key.vertex() < _label_informations.size()
			   and _label_informations.at(steiner_tree_key.vertex()).exists(steiner_tree_key.terminals())
			   and _label_informations.at(steiner_tree_key.vertex()).at(steiner_tree_key.terminals()).initialised;
	}

	LabelInformation const& get_label_information(SteinerTreeKey<num_terminals> const& steiner_tree_key) const
	{
		return _label_informations.at(steiner_tree_key.vertex()).at(steiner_tree_key.terminals());
	}

	void set_label_information(Key const& key, Cost const& cost, Backtrack const& backtrack)
	{
		if (key.vertex() >= _label_informations.size()) {
			_label_informations.resize(key.vertex() + 1);
		}

		_label_informations.at(key.vertex()).set(key.terminals(), LabelInformation(true, cost, backtrack));
	}

private:
	std::vector<BitSetTable<num_terminals, LabelInformation>> _label_informations;
};

} // namespace dijkstra_steiner
} // namespace steiner_trees

#endif
