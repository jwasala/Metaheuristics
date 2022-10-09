#pragma once
#include <vector>

#include "ttp.h"

class ttp_solution
{
private:
	std::vector<int> nodes_ids_order_;
	std::vector<bool> is_item_picked_;
	ttp& ttp_;
public:
	ttp_solution(std::vector<int> nodes_ids_order, std::vector<bool> is_item_picked, ttp& ttp)
		: nodes_ids_order_(std::move(nodes_ids_order)),
		  is_item_picked_(std::move(is_item_picked)),
		  ttp_(ttp)
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

	[[nodiscard]] ttp& ttp() const
	{
		return ttp_;
	}

	double fitness() const;

	static ttp_solution make_random(::ttp& ttp);
};
