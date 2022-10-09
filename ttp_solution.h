#pragma once
#include <vector>

class ttp_solution
{
private:
	std::vector<int> nodes_ids_order_;
	std::vector<bool> is_item_picked_;
public:
	ttp_solution(std::vector<int> nodes_ids_order, std::vector<bool> is_item_picked)
		: nodes_ids_order_(std::move(nodes_ids_order)),
		  is_item_picked_(std::move(is_item_picked))
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
};
