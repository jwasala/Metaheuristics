#include "ttp.h"

#include <memory>
#include <random>
#include <ranges>

double ttp_node::dist_to(ttp_node const& other) const
{
	const auto dx = x() - other.x();
	const auto dy = y() - other.y();
	return dx * dx + dy * dy;
}

ttp::ttp(std::string problem_name, std::string knapsack_data_type, const int dimension, const int number_of_items,
	const int capacity_of_knapsack, const double min_speed, const double max_speed, const double renting_ratio,
	std::string edge_weight_type, std::map<int, ttp_node> nodes, std::map<int, ttp_item> items): problem_name_(std::move(problem_name)),
	knapsack_data_type_(std::move(knapsack_data_type)),
	dimension_(dimension),
	number_of_items_(number_of_items),
	capacity_of_knapsack_(capacity_of_knapsack),
	min_speed_(min_speed),
	max_speed_(max_speed),
	renting_ratio_(renting_ratio),
	edge_weight_type_(std::move(edge_weight_type)),
	nodes_(std::move(nodes)),
	items_(std::move(items))
{
	distances_ = make_distance_matrix();
	items_at_node_ = make_items_at_node_mapping();
}

std::vector<std::vector<double>> ttp::make_distance_matrix()
{
	const auto size = nodes_.size();
	std::vector<std::vector<double>> mtx(size, std::vector<double>(size));

	for (int i = 1; i <= size; i++)
	{
		for (int j = 1; j <= size; j++)
		{
			mtx[i - 1][j - 1] = nodes().at(i).dist_to(nodes().at(j));
		}
	}

	return mtx;
}

std::map<int, std::vector<int>> ttp::make_items_at_node_mapping()
{
	std::map<int, std::vector<int>> map;

	for (auto& item : items() | std::views::values)
	{
		map[item.node_id()].push_back(item.id());
	}

	return map;
}

std::string ttp::dumps() const
{
	return "Problem " + std::to_string(dimension()) + "/" + std::to_string(number_of_items()) + "/" + std::to_string(capacity_of_knapsack());
}

double ttp::fitness(const ttp_solution& solution)
{
	double current_weight = 0;
	double current_profit = 0;
	double current_time = 0;

	for (int i = 0; i < this->dimension(); i++)
	{
		const auto from_id = solution.nodes_ids_order()[i];
		int to_id;
		if (i == this->dimension() - 1)
		{
			to_id = solution.nodes_ids_order()[0];
		}
		else
		{
			to_id = solution.nodes_ids_order()[i + 1];
		}

		for (const auto item_id : this->items_at(from_id))
		{
			auto& item = this->items().at(item_id);
			current_weight += item.weight();
			current_profit += item.profit();
		}

		const double current_speed = this->max_speed() - (current_weight * this->speed_weight_quotient());
		current_time += this->distance_between(from_id, to_id) / current_speed;
	}

	return current_profit - current_time;
}

ttp_solution ttp::make_random_solution()
{
	std::random_device rd;
	std::mt19937 g(rd());
	const int items_count = this->number_of_items();
	const int nodes_count = this->dimension();

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
	std::vector<bool> is_item_picked(this->number_of_items(), false);
	int total_weight = 0;
	for (const auto& item_id : items_ids)
	{
		if (auto& item = this->items().at(item_id); total_weight + item.weight() <= this->capacity_of_knapsack())
		{
			is_item_picked.at(item_id - 1) = true;
			total_weight += item.weight();
		}
	}

	return ttp_solution{ nodes_ids, is_item_picked };
}
