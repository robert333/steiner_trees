#include "Logger.hpp"

#include <debug.hpp>

std::ostream* Logger::_ostream_ptr = nullptr;

void Logger::set_logger(std::ostream& ostream)
{
	_ostream_ptr = &ostream;
}

std::ostream& Logger::logger()
{
	assert(_ostream_ptr != nullptr);
	return *_ostream_ptr;
}
