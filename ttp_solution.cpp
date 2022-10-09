#include "ttp_solution.h"

#include <random>

double ttp_solution::fitness() const
{
	double current_weight = 0;
	double current_profit = 0;
	double current_time = 0;

	for (int i = 0; i < ttp().dimension(); i++)
	{
		const auto from_id = nodes_ids_order()[i];
		int to_id;
		if (i == ttp().dimension() - 1)
		{
			to_id = nodes_ids_order()[0];
		} else
		{
			to_id = nodes_ids_order()[i + 1];
		}
		
		for (const auto item_id : ttp().items_at(from_id))
		{
			auto& item = ttp().items().at(item_id);
			current_weight += item.weight();
			current_profit += item.profit();
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
	std::vector<bool> is_item_picked(ttp.number_of_items(), false);
	int total_weight = 0;
	for (const auto& item_id : items_ids)
	{
		if (auto& item = ttp.items().at(item_id); total_weight + item.weight() <= ttp.capacity_of_knapsack())
		{
			is_item_picked.at(item_id - 1) = true;
			total_weight += item.weight();
		}
	}

	return ttp_solution{nodes_ids, is_item_picked, ttp};
}
