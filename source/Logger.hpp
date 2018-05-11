#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <ostream>

class Logger {
public:
	static void set_logger(std::ostream& ostream);
	static std::ostream& logger();

private:
	static std::ostream* _ostream_ptr;
};

#endif
