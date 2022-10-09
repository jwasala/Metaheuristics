#include <filesystem>
#include <iostream>
#include <vector>

#include "config_loader.h"
#include "ttp_loader.h"
#include "ttp_solution.h"

int main(int argc, char* argv[])
{
	const auto config = config_loader(argv[1]).load();
	std::vector<ttp> problems;
	for (auto const& dir_entry : std::filesystem::directory_iterator{config.problem_files_directory()})
	{
		auto problem = ttp_loader(dir_entry.path().string()).load();
		problems.push_back(problem);
	}

	const auto start_time_make_random = std::chrono::high_resolution_clock::now();

	std::map<ttp*, std::vector<ttp_solution>> problems_solutions;
	for (auto& problem : problems)
	{
		for (int i = 0; i < 1000; i++)
		{
			if (!problems_solutions.contains(&problem))
			{
				problems_solutions.insert({ &problem, std::vector<ttp_solution>() });
				problems_solutions.at(&problem).reserve(1000);
			}
			problems_solutions.at(&problem).push_back(problem.make_random_solution());
		}
	}

	const auto end_time_make_random = std::chrono::high_resolution_clock::now();

	for (auto& [problem, solutions] : problems_solutions)
	{
		for (auto& solution : solutions)
		{
			problem->fitness(solution);
		}
	}

	const auto end_time_fitness = std::chrono::high_resolution_clock::now();

	const auto make_random_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		end_time_make_random - start_time_make_random);
	const auto fitness_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		end_time_fitness - end_time_make_random);

	std::cout << "make_random [ms]: " << make_random_ms << std::endl;
	std::cout << "fitness [ms]: " << fitness_ms << std::endl;

	return 0;
}
