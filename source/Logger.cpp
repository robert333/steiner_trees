#include "Logger.hpp"

std::shared_ptr<std::ostream> Logger::_ostream_ptr;

void Logger::set_logger(std::shared_ptr<std::ostream> const& ostream_ptr)
{
	_ostream_ptr = ostream_ptr;
}

std::ostream& Logger::logger()
{
	return _ostream_ptr.operator*();
}
