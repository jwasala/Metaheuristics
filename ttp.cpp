#include "ttp.h"

#include <memory>
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
