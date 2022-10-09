#include "ttp_solution.h"

#include <iostream>
#include <random>
#include <ranges>

double ttp_solution::fitness()
{
	double current_weight = 0;
	double current_profit = 0;
	double current_time = 0;
	const auto& items = ttp().items();

	for (auto& route_part : route_parts())
	{
		const auto from_id = route_part.node_from_id();
		const auto to_id = route_part.node_to_id();

		for (const auto& item : route_part.item_ids())
		{
			current_weight += items.at(item).weight();
			current_profit += items.at(item).profit();
		}

		const double current_speed = ttp().max_speed() - (current_weight * ttp().speed_weight_quotient());
		current_time += ttp().distance_between(from_id, to_id) / current_speed;
	}

	return current_profit - current_time;
}

ttp_solution ttp_solution::make_random(::ttp& ttp)
{
	std::random_device rd;
	std::mt19937 g(rd());
	const int items_count = ttp.number_of_items();
	const int nodes_count = ttp.dimension();

	// Get items IDs in random order
	std::vector<int> items_ids;
	items_ids.reserve(items_count);
	for (int i = 0; i < items_count; i++)
	{
		items_ids.push_back(i + 1);
	}
	std::ranges::shuffle(items_ids, g);

	// Get nodes IDs in random order
	std::vector<int> nodes_ids;
	nodes_ids.reserve(nodes_count);
	for (int i = 0; i < nodes_count; i++)
	{
		nodes_ids.push_back(i + 1);
	}
	std::ranges::shuffle(nodes_ids, g);

	// Pick items within weight limit
	std::vector<int> picked_items_ids;
	picked_items_ids.reserve(items_count);
	std::map<int, std::vector<int>> items_for_node;
	int total_weight = 0;
	for (const auto& item_id : items_ids)
	{
		if (auto& item = ttp.items().at(item_id); total_weight + item.weight() <= ttp.capacity_of_knapsack())
		{
			picked_items_ids.push_back(item_id);

			if (items_for_node.contains(item.node_id()))
			{
				items_for_node.at(item.node_id()).push_back(item_id);
			}
			else
			{
				items_for_node.insert({item.node_id(), std::vector<int>{item_id}});
			}

			total_weight += item.weight();
		}
	}

	// Turn nodes into route_parts.
	std::vector<ttp_route_part> route_parts;
	route_parts.reserve(nodes_ids.size() - 1);

	for (int i = 0; i < nodes_ids.size() - 1; i++)
	{
		auto node_id_from = nodes_ids.at(i);
		auto node_id_to = nodes_ids.at(i + 1);

		if (items_for_node.contains(node_id_from))
		{
			route_parts.emplace_back(node_id_from, node_id_to, items_for_node.at(node_id_from));
		}
		else
		{
			route_parts.emplace_back(node_id_from, node_id_to, std::vector<int>());
		}
	}

	return ttp_solution{route_parts, ttp};
}
