#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <memory>

class Logger {
public:
	static void set_logger(std::shared_ptr<std::ostream> const& ostream_ptr);
	static std::ostream& logger();

private:
	static std::shared_ptr<std::ostream> _ostream_ptr;
};

#endif
