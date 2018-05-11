// debug.hpp
#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <cassert>
#include <iostream>
#include <vector>
#include <deque>

#include "helper/vector.hpp"

#define DUMMY_USE(x) ((void)(x))
#define FORBIDDEN throw

//#define DEBUG

#ifdef NDEBUG
inline void debug_output(std::string const&)
{}
#else
inline void debug_output(std::string const& text)
{
	std::cout << text;
}
#endif

template<typename T>
void output(T const& object, std::string const& name = "")
{
	if (not name.empty()) {
		std::cout << name << " = ";
	}

	std::cout << object << "\n";
}

template<typename T>
void output(std::vector<T> const& vector, std::string const& name = "")
{
	std::cout << helper::to_string(vector, name) << "\n";
}

template<typename T>
void output(std::deque<T> const& deque, std::string const& name = "")
{
	if (not name.empty()) {
		std::cout << name << " = ";
	}

	std::cout << "{";

	if (not deque.empty()) {
		std::cout << deque[0];

		for (std::size_t i = 1; i < deque.size(); ++i) {
			std::cout << ", " << deque[i];
		}
	}

	std::cout << "}\n";
}

#endif
