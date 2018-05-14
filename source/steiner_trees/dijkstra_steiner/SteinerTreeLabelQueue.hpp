#ifndef STEINERTREES_DIJKSTRASTEINER_STEINERTREELABELQUEUE_HPP
#define STEINERTREES_DIJKSTRASTEINER_STEINERTREELABELQUEUE_HPP

#include <set>
#include <queue>
#include <functional>

#include "SteinerTreeLabel.hpp"

namespace steiner_trees {
namespace dijkstra_steiner {

template<std::size_t num_terminals>
class SteinerTreeLabelQueue {
public:
	using Label = SteinerTreeLabel<num_terminals>;

public:
	bool is_empty() const
	{
		return _queue.empty();
	}

	Label top_and_pop()
	{
		assert(not is_empty());
		Label top_label = *_queue.begin();
#ifndef NDEBUG
		std::size_t size = _queue.size();
#endif
		_queue.erase(_queue.begin());
		assert(_queue.size() + 1 == size);
		return top_label;
	}

	void insert(Label const& label)
	{
		_queue.insert(label);
	}

private:
	bool exists(Label const& label) const
	{
		return _queue.count(label) == 1;
	}

private:
//	std::priority_queue<Label, std::vector<Label>, std::function<bool(Label const&, Label const&)>> _queue;
//	std::set<Label, std::function<bool(Label const&, Label const&)>> _queue;
	std::set<Label> _queue;
};

} // namespace dijkstra_steiner
} // namespace steiner_trees

#endif
