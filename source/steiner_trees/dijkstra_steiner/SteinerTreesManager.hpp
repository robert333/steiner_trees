#ifndef STEINERTREES_DIJKSTRASTEINER_STEINERTREESMANAGER_HPP
#define STEINERTREES_DIJKSTRASTEINER_STEINERTREESMANAGER_HPP

#include <limits>
#include <queue>
#include <array>
#include <functional>

#include "SteinerTreeLabelTable.hpp"
#include "SteinerTreeLabelQueue.hpp"

namespace steiner_trees {
namespace dijkstra_steiner {

template<std::size_t num_terminals>
class SteinerTreesManager {
public:
	using Key = SteinerTreeKey<num_terminals>;
	using Label = SteinerTreeLabel<num_terminals>;
	using Cost = typename Label::Cost;
	using Backtrack = typename Label::Backtrack;

public:
	SteinerTreesManager(typename Label::Key::Vertex const& root_terminal) :
		_root_terminal(root_terminal)
	{}

	/**
	 * Computes the next valid label in the queue/heap and saves it in the given reference
	 * @return false iff we reach the goal label (root, T - root)
	 */
	bool next(Label& label)
	{
		assert(not _steiner_tree_label_queue.is_empty());

		// since we have also outdated labels in our queue due to our update routine (see below)
		// we need to check each label if it is processed already with a smaller value
		do {
			label = _steiner_tree_label_queue.top_and_pop();
		} while (_steiner_tree_label_table.is_processed(label.key()));

		// mark label as processed
		_steiner_tree_label_table.set_label_information(label.key(), label.cost(), label.backtrack());

		return label.key().vertex() != _root_terminal or
			   label.key().terminals().num_ones() != label.key().terminals().size();
	}

	bool is_processed(Key const& key) const
	{
		return _steiner_tree_label_table.is_processed(key);
	}

	/**
	 * "updates" for the given label the cost in the queue/heap
	 * @return false if we want to update a processed label
	 */
	bool update(Label const& label)
	{
		// we are not doing a decrease key operation
		// instead we are just inserting the new key
		// then the right label will be selected first due to the minimum queue
		// and we ignore all other "same" labels after that
		if (not is_processed(label.key())) {
			_steiner_tree_label_queue.insert(label);
		}

		return false;
	}

	Cost get_minimum_cost(Key const& key) const
	{
		assert(_steiner_tree_label_table.get_label_information(key).initialised);
		return _steiner_tree_label_table.get_label_information(key).minimum_cost;
	}

	Backtrack get_backtrack(Key const& key) const
	{
		assert(_steiner_tree_label_table.get_label_information(key).initialised);
		return _steiner_tree_label_table.get_label_information(key).backtrack;
	}

private:
	typename Label::Key::Vertex _root_terminal;

	SteinerTreeLabelTable<num_terminals> _steiner_tree_label_table;
	SteinerTreeLabelQueue<num_terminals> _steiner_tree_label_queue;
};

} // namespace dijkstra_steiner
} // namespace steiner_trees

#endif
