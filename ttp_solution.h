#pragma once
#include <vector>

#include "ttp.h"

class ttp_route_part
{
private:
	int node_from_id_;
	int node_to_id_;
	std::vector<int> item_ids_;
public:
	ttp_route_part(const int node_from_id, const int node_to_id, std::vector<int> item_ids)
		: node_from_id_(node_from_id),
		  node_to_id_(node_to_id),
		  item_ids_(std::move(item_ids))
	{
	}

	[[nodiscard]] int node_from_id() const
	{
		return node_from_id_;
	}

	[[nodiscard]] int node_to_id() const
	{
		return node_to_id_;
	}

	[[nodiscard]] std::vector<int>& item_ids()
	{
		return item_ids_;
	}
};

class ttp_solution
{
private:
	std::vector<ttp_route_part> route_parts_;
	ttp& ttp_;
public:
	ttp_solution(std::vector<ttp_route_part> route_parts, ttp& ttp)
		: route_parts_(std::move(route_parts)),
		  ttp_(ttp)
	{
	}

	[[nodiscard]] std::vector<ttp_route_part>& route_parts()
	{
		return route_parts_;
	}

	[[nodiscard]] ttp& ttp() const
	{
		return ttp_;
	}

	double fitness();

	static ttp_solution make_random(::ttp& ttp);
};
