#include "config_loader.h"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

config config_loader::load() const
{
	std::ifstream file(filename());
	if (!file.good())
	{
		throw std::runtime_error("File could not be read.");
	}
	json data = json::parse(file);

	try
	{
		return config{
			data["problemFilesDirectory"]
		};
	} catch (std::exception& e)
	{
		const auto new_message = "Error while parsing config file. Data is missing or has wrong type: ";
		throw std::runtime_error(new_message + *e.what());
	}
}
