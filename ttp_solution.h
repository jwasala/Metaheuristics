#pragma once
#include <vector>

class ttp_solution
{
private:
	std::vector<bool> is_item_picked_;
	double fitness_cached_ = 0.0;
public:
	std::vector<int> nodes_ids_order_;

	ttp_solution(std::vector<int> nodes_ids_order, std::vector<bool> is_item_picked)
		: is_item_picked_(std::move(is_item_picked)),
		  nodes_ids_order_(std::move(nodes_ids_order))
	{
	}

	[[nodiscard]] std::vector<int> nodes_ids_order() const
	{
		return nodes_ids_order_;
	}

	[[nodiscard]] std::vector<bool> is_item_picked() const
	{
		return is_item_picked_;
	}

	[[nodiscard]] double fitness() const;

	[[nodiscard]] double fitness_cached() const
	{
		return fitness_cached_;
	}

	void set_fitness_cached(const double fitness_cached)
	{
		fitness_cached_ = fitness_cached;
	}
};
