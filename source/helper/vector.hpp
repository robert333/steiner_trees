#ifndef HELPER_VECTOR_HPP
#define HELPER_VECTOR_HPP

#include <vector>
#include <deque>
#include <algorithm>
#include <string>
#include <sstream>

namespace helper {

template<typename T>
bool is_unique(std::vector<T> const& vector)
{
	std::vector<T> vector_copy = vector;
	std::sort(vector_copy.begin(), vector_copy.end());
	return std::unique(vector_copy.begin(), vector_copy.end()) == vector_copy.end();
}

template<typename T>
bool is_unique(std::deque<T> const& deque)
{
	std::deque<T> deque_copy = deque;
	std::sort(deque_copy.begin(), deque_copy.end());
	return std::unique(deque_copy.begin(), deque_copy.end()) == deque_copy.end();
}

template<typename T>
void sort(std::vector<T>& vector)
{
	std::sort(vector.begin(), vector.end());
}

template<typename T>
void sort_and_unique_and_erase(std::vector<T>& vector)
{
	sort(vector);
	vector.erase(std::unique(vector.begin(), vector.end()), vector.end());
}

template<typename T>
typename std::vector<T>::const_iterator find(std::vector<T> const& vector, T const& object)
{
	return std::find(vector.begin(), vector.end(), object);
}

template<typename T>
bool contains(std::vector<T> const& vector, T const& object)
{
	return find(vector, object) != vector.end();
}

template<typename T>
typename std::vector<T>::const_iterator erase(std::vector<T>& vector, T const& object)
{
	return vector.erase(find(vector, object));
}

template<typename T>
void append(std::vector<T>& vector, std::vector<T> const& vector_appending)
{
	vector.insert(vector.end(), vector_appending.begin(), vector_appending.end());
}

template<typename T>
bool are_disjoint(std::vector<T> const& vector_a, std::vector<T> const& vector_b)
{
	std::vector<T> vector_copy = vector_a;
	append(vector_copy, vector_b);
	return is_unique(vector_copy);
}

template<typename T>
std::string to_string(std::vector<T> const& vector, std::string const& name = "")
{
	std::stringstream stringstream;

	if (not name.empty()) {
		stringstream << name << " = ";
	}

	stringstream << "{";

	if (not vector.empty()) {
		stringstream << vector.at(0);

		for (std::size_t i = 1; i < vector.size(); ++i) {
			stringstream << ", " << vector.at(i);
		}
	}

	stringstream << "}";

	return stringstream.str();
}

} // namespace helper

#endif
