#include "json.hpp"

#include <fstream>

json read_json(std::istream& istream)
{
	json result;
	remove_comments(istream) >> result;
	return result;
}

void write_json(std::ostream& ostream, json const& data)
{
	ostream << std::setw(4) << data << std::endl;
}

json read_json(std::string const& input_file_path)
{
	std::ifstream input_file(input_file_path);

	if (not input_file.is_open()) {
		throw;
	}

	return read_json(input_file);
}

void write_json(std::string const& output_file_path, json const& data)
{
	std::ofstream output_file(output_file_path);

	if (not output_file.is_open()) {
		throw;
	}

	write_json(output_file, data);
}

std::stringstream remove_comments(std::istream& istream)
{
	std::stringstream result;

	std::string line;
	while (getline(istream, line)) {
		std::size_t const pos = line.find("//");
		if (pos != std::string::npos) {
			line.erase(pos);
		}
		result << line << "\n";
	}

	return result;
}
