#ifndef JSON_HPP
#define JSON_HPP

#include "../json/json.hpp"

using json = nlohmann::json;

json read_json(std::istream& istream);
void write_json(std::ostream& ostream, json const& data);

json read_json(std::string const& input_file_path);
void write_json(std::string const& output_file_path, json const& data);

std::stringstream remove_comments(std::istream& istream);

#endif
