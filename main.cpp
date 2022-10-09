#include <filesystem>
#include <iostream>
#include <random>
#include <set>
#include <vector>

#include "config_loader.h"
#include "ea.h"
#include "ttp_loader.h"
#include "ttp_solution.h"



std::mt19937_64 random_engine{std::random_device{}()};

// Operators implementations
auto select_tournament = [](ea<ttp_solution>& ea, std::vector<ttp_solution>& solutions)
{
	const std::uniform_int_distribution<int> dist(0, solutions.size() - 1);
	std::optional<ttp_solution> best_solution;
	double best_solution_fitness = std::numeric_limits<double>::min();

	for (int i = 0; i < ea.tour(); i++)
	{
		ttp_solution& random_solution = solutions.at(dist(random_engine));
		const double random_solution_fitness = ea.problem().fitness(random_solution);
		if (random_solution_fitness > best_solution_fitness)
		{
			best_solution_fitness = random_solution_fitness;
			best_solution = random_solution;
		}
	}
	if (best_solution.has_value())
	{
		return best_solution.value();
	}
	return solutions.at(0);
};

auto mutate_swap = [](ea<ttp_solution>& ea, ttp_solution const& solution)
{
	const std::uniform_int_distribution<int> dist(0, solution.nodes_ids_order().size() - 2);
	ttp_solution mutated_solution = solution;
	mutated_solution.set_fitness_cached(0.0);
	for (int i = 0; i < 55; i++)
	{
		const int swap_left = dist(random_engine);
		const int swap_right = dist(random_engine);
		std::iter_swap(mutated_solution.nodes_ids_order_.begin() + swap_left,
			mutated_solution.nodes_ids_order_.begin() + swap_right);
	}
	return mutated_solution;
};

auto crossover_ox = [](ea<ttp_solution>&, ttp_solution& sln1, ttp_solution& sln2)
{
	const std::uniform_int_distribution<int> dist(0, sln1.nodes_ids_order().size() - 2);

	int start_left = dist(random_engine);
	int end_left = dist(random_engine);
	if (start_left > end_left)
	{
		const int temp = start_left;
		start_left = end_left;
		end_left = temp;
	}

	std::set<int> picked_nodes_from_left(sln1.nodes_ids_order_.begin() + start_left, sln1.nodes_ids_order_.begin() + end_left);

	std::vector<int> nodes_ids;
	const int nodes_count = sln1.nodes_ids_order().size();
	nodes_ids.reserve(nodes_count);
	int right_id = 0;

	for (int i = 0; i < nodes_count; i++)
	{
		if (i >= start_left && i < end_left)
		{
			nodes_ids.push_back(sln1.nodes_ids_order_.at(i));
		} else
		{
			while (right_id < nodes_count && picked_nodes_from_left.contains(sln2.nodes_ids_order_.at(right_id)))
			{
				right_id++;
			}
			if (right_id < nodes_count)
			{
				nodes_ids.push_back(sln2.nodes_ids_order_.at(right_id));
				right_id++;
			}
		}
	}

	if (nodes_count != nodes_ids.size())
	{
		throw std::runtime_error("Not every node has been added to outcome vector");
	}

	return ttp_solution{nodes_ids, sln1.is_item_picked()};
};

int main(int argc, char* argv[])
{
	const auto config = config_loader(argv[1]).load();
	std::vector<ttp> problems;
	for (auto const& dir_entry : std::filesystem::directory_iterator{config.problem_files_directory()})
	{
		auto problem = ttp_loader(dir_entry.path().string()).load();
		problems.push_back(problem);
	}

	auto problem = problems.at(6);

	ea<ttp_solution> evolutionary_algorithm(problem,
	                                        crossover_ox,
	                                        "OX",
	                                        mutate_swap,
	                                        "SWAP",
	                                        select_tournament,
	                                        "TOUR",
	                                        100,
	                                        100,
	                                        0.7,
	                                        0.1,
	                                        5);

	evolutionary_algorithm.solve();

	return 0;
}
