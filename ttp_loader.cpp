#include "ttp_loader.h"

#include <fstream>
#include <sstream>
#include <optional>

std::tuple<std::string, std::string> ttp_loader::parse_line(const std::string& line)
{
	const auto semi_pos = line.find(':');
	const auto key = line.substr(0, semi_pos);
	if (semi_pos == std::string::npos)
	{
		throw ttp_loader_error("Bad line in file.");
	}
	std::string value = line.substr(semi_pos + 1);
	value.erase(0, value.find_first_not_of((" \n\r\t")));
	return {key, value};
}

ttp_node ttp_loader::get_node_value(const std::string& line)
{
	std::stringstream ss(line);

	try
	{
		std::string id_str, x_str, y_str;
		ss >> id_str;
		ss >> x_str;
		ss >> y_str;
		return ttp_node{std::stoi(id_str), std::stod(x_str), std::stod(y_str)};
	}
	catch (std::exception&)
	{
		throw ttp_loader_error("Node value could not be parsed: " + line);
	}
}

ttp_item ttp_loader::get_item_value(const std::string& line)
{
	std::stringstream ss(line);
	try
	{
		std::string id_str, profit_str, weight_str, node_id_str;
		ss >> id_str;
		ss >> profit_str;
		ss >> weight_str;
		ss >> node_id_str;
		return ttp_item{std::stoi(id_str), std::stoi(profit_str), std::stoi(weight_str), std::stoi(node_id_str)};
	}
	catch (std::exception&)
	{
		throw ttp_loader_error("Item value could not be parsed: " + line);
	}
}

ttp ttp_loader::load() const
{
	std::ifstream file(filename());
	if (!file.good())
	{
		throw ttp_loader_error("File could not be read.");
	}

	std::map<int, ttp_node> nodes;
	std::map<int, ttp_item> items;
	std::map<std::string, std::string> lines;

	std::string line;
	int section = 0; // 0 - before list sections, 1 - in node section, 2 - in item section
	while (std::getline(file, line))
	{
		if (line.starts_with("NODE_COORD_SECTION"))
		{
			section = 1;
		}
		else if (line.starts_with("ITEMS SECTION"))
		{
			section = 2;
		}
		else if (section == 1)
		{
			auto node_obj = get_node_value(line);
			nodes.insert({node_obj.id(), node_obj});
		}
		else if (section == 2)
		{
			auto item_obj = get_item_value(line);
			items.insert({item_obj.id(), item_obj});
		}
		else
		{
			auto&& [key, value] = parse_line(line);
			lines.insert({key, value});
		}
	}
	
		return ttp{
		lines["PROBLEM NAME"],
		lines["KNAPSACK DATA TYPE"],
		std::stoi(lines["DIMENSION"]),
		std::stoi(lines["NUMBER OF ITEMS"]),
		std::stoi(lines["CAPACITY OF KNAPSACK"]),
		std::stod(lines["MIN SPEED"]),
		std::stod(lines["MAX SPEED"]),
		std::stod(lines["RENTING RATIO"]),
		lines["EDGE_WEIGHT_TYPE"],
		nodes,
		items
		};
}
