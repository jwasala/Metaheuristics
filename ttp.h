#pragma once
#include <map>
#include <string>
#include <vector>

#include "base_algorithm.h"
#include "ttp_solution.h"

class ttp_node
{
private:
	int id_;
	double x_;
	double y_;
public:
	ttp_node(const int id, const double x, const double y)
		: id_(id),
		  x_(x),
		  y_(y)
	{
	}

	[[nodiscard]] int id() const
	{
		return id_;
	}

	[[nodiscard]] double x() const
	{
		return x_;
	}

	[[nodiscard]] double y() const
	{
		return y_;
	}

	[[nodiscard]] double dist_to(ttp_node const& other) const;
};

class ttp_item
{
private:
	int id_;
	int profit_;
	int weight_;
	int node_id_;
public:
	ttp_item(const int id, const int profit, const int weight, const int node_id)
		: id_(id),
		  profit_(profit),
		  weight_(weight),
		  node_id_(node_id)
	{
	}

	[[nodiscard]] int id() const
	{
		return id_;
	}

	[[nodiscard]] int profit() const
	{
		return profit_;
	}

	[[nodiscard]] int weight() const
	{
		return weight_;
	}

	[[nodiscard]] int node_id() const
	{
		return node_id_;
	}
};

class ttp : public solvable<ttp_solution>
{
private:
	std::string problem_name_;
	std::string knapsack_data_type_;
	int dimension_;
	int number_of_items_;
	int capacity_of_knapsack_;
	double min_speed_;
	double max_speed_;
	double renting_ratio_;
	std::string edge_weight_type_;
	std::map<int, ttp_node> nodes_;
	std::map<int, ttp_item> items_;

	std::vector<std::vector<double>> distances_;
	std::map<int, std::vector<int>> items_at_node_;

public:
	ttp(std::string problem_name, std::string knapsack_data_type, const int dimension,
	    const int number_of_items, const int capacity_of_knapsack, const double min_speed, const double max_speed,
	    const double renting_ratio, std::string edge_weight_type, std::map<int, ttp_node> nodes,
	    std::map<int, ttp_item> items);

	std::vector<std::vector<double>> make_distance_matrix();

	std::map<int, std::vector<int>> make_items_at_node_mapping();

	[[nodiscard]] double fitness(ttp_solution& solution) override;

	ttp_solution make_random_solution() override;

	[[nodiscard]] std::string dumps() const;

	[[nodiscard]] std::string const& problem_name() const
	{
		return problem_name_;
	}

	[[nodiscard]] std::string const& knapsack_data_type() const
	{
		return knapsack_data_type_;
	}

	[[nodiscard]] int dimension() const
	{
		return dimension_;
	}

	[[nodiscard]] int number_of_items() const
	{
		return number_of_items_;
	}

	[[nodiscard]] int capacity_of_knapsack() const
	{
		return capacity_of_knapsack_;
	}

	[[nodiscard]] double speed_weight_quotient() const
	{
		return (max_speed() - min_speed()) / capacity_of_knapsack();
	}

	[[nodiscard]] double distance_between(const int node_1_id, const int node_2_id) const;

	[[nodiscard]] std::vector<int>& items_at(const int node_id);

	[[nodiscard]] double min_speed() const
	{
		return min_speed_;
	}

	[[nodiscard]] double max_speed() const
	{
		return max_speed_;
	}

	[[nodiscard]] double renting_ratio() const
	{
		return renting_ratio_;
	}

	[[nodiscard]] std::string const& edge_weight_type() const
	{
		return edge_weight_type_;
	}

	[[nodiscard]] std::map<int, ttp_node>& nodes()
	{
		return nodes_;
	}

	[[nodiscard]] std::map<int, ttp_item>& items()
	{
		return items_;
	}
};
