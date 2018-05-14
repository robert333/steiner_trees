// BinaryHeap.hpp
#ifndef BINARYHEAP_HPP
#define BINARYHEAP_HPP

#include <vector>
#include <iostream>

//
// Class for managing a binary heap
// with an label and a key
//
template<typename Label, typename Key>
class BinaryHeap {
public:
	BinaryHeap(std::size_t n_max);

	void insert(Label const& label, Key const& key);
	void decreaseKey(Label const& label, Key const& key);
	Label deleteMin();
	Label deleteMin_();

	void print();

private:
	struct Element {
		Label label;
		Key key;

		bool operator<(Element const& element) const {
			return key < element.key;
		}
	};

private:
	std::size_t n;

	std::vector<Element> elements;
	std::vector<std::size_t> positions;
};

template<typename Label, typename Key>
BinaryHeap<Label, Key>::BinaryHeap(std::size_t n_max) :
	n(1),
	elements(n_max + 1),
	positions(n_max)
{

}

template<typename Label, typename Key>
void BinaryHeap<Label, Key>::insert(Label const& label, Key const& key)
{
	// initialize current position in heap
	// and increase size by one
	std::size_t i = n++;

	// initialize current parent position in heap
	std::size_t p = i / 2;

	// create new element
	Element element{label, key};

	// while current position is not root and element is less than parent
	// we must go up in binary min heap
	while (i != 1 and element < elements[p]) {
		// get label of parent
		Label label = elements[p].label;

		// move parent one level down
		elements[i] = elements[p];
		positions[label] = i;

		// update current position and parent
		i = p;
		p = i / 2;
	}

	// insert new element at correct position
	elements[i] = element;
	positions[label] = i;
}

template<typename Label, typename Key>
void BinaryHeap<Label, Key>::decreaseKey(Label const& label, Key const& key)
{
	// initialize current position in heap
	std::size_t i = positions[label];

	// initialize current parent position in heap
	std::size_t p = i / 2;

	// get specific element
	Element element = elements[i];

	// update element key
	element.key = key;

	// while current position is not root and element is less than parent
	// we must go up in binary min heap
	while (i != 1 and element < elements[p]) {
		// get label of parent
		Label label = elements[p].label;

		// move parent one level down
		elements[i] = elements[p];
		positions[label] = i;

		// update current position and parent
		i = p;
		p = i / 2;
	}

	// set element to correct position
	elements[i] = element;
	positions[label] = i;
}

template<typename Label, typename Key>
Label BinaryHeap<Label, Key>::deleteMin()
{
	if (n > 1) {
		// get minimum label
		Label min = elements[1].label;

		// initialize current position in heap
		std::size_t i = 1;

		// initialize current left child position in heap
		std::size_t c = 2 * i;

		// move minimum to the bottom of binary heap
		while (c < n) {
			// if child is not last element and right child is less than left one
			// go to right child in binary heap
			if (c != n - 1 and elements[c + 1] < elements[c])
				++c;

			// get label of child
			Label label = elements[c].label;

			// move child one level up
			elements[i] = elements[c];
			positions[label] = i;

			// update current position and child
			i = c;
			c = 2 * i;
		}

		// if current position is not the end of the heap
		// move last element up to correct position
		// anyway decrease size of heap by one
		if (i != --n) {
			// get last element
			Element element = elements[n];

			// initialize current parent position in heap
			std::size_t p = i / 2;

			// while current position is not root and element is less than parent
			// we must go up in binary min heap
			while (i != 1 and element < elements[p]) {
				// get label of parent
				Label label = elements[p].label;

				// move parent one level down
				elements[i] = elements[p];
				positions[label] = i;

				// update current position and parent
				i = p;
				p = i / 2;
			}

			// get label of element
			Label label = element.label;

			// set element to correct position
			elements[i] = element;
			positions[label] = i;
		}

		return min;
	}

	return -1;
}

template<typename Label, typename Key>
Label BinaryHeap<Label, Key>::deleteMin_()//debug faster???
{
	if (n > 1) {
		// get minimum label
		Label min = elements[1].label;

		// get last element
		// and decrease size of heap by one
		Element element = elements[--n];

		// initialize current position in heap
		std::size_t i = 1;

		// initialize current left child position in heap
		std::size_t c = 2 * i;

		// move last element to correct position in heap
		while (c < n) {
			// get lesser child
			if (elements[c + 1] < elements[c])
				++c;

			// compare last element with lesser child
			if (elements[c] < element) {
				// get label of child
				Label label = elements[c].label;

				// move child one level up
				elements[i] = elements[c];
				positions[label] = i;

				// update current position and child
				i = c;
				c = 2 * i;
			} else
				break;//debug
		}

		// get label of last element
		Label label = element.label;

		// set last element to correct position
		elements[i] = element;
		positions[label] = i;

		return min;
	}

	return -1;
}

template<typename Label, typename Key>
void BinaryHeap<Label, Key>::print()
{
	std::cout << "\n(label, key, pos_s, pos) n = " << n << '\n';

	for (std::size_t i = 1; i < n; i *= 2) {
		for (std::size_t j = 0; j < i and j + i < n; ++j)
			std::cout << "(" << elements[i + j].label << ", " << elements[i + j].key << ", "
					  << positions[elements[i + j].label] << ", " << i + j << ") ";
		std::cout << '\n';
	}
}

#endif
