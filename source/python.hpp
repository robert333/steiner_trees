#ifndef PYTHON_HPP
#define PYTHON_HPP

#include <boost/python.hpp>

char const* greet();

BOOST_PYTHON_MODULE(steiner_trees_python_lib)
{
	boost::python::def("greet", greet);
}

#endif
