#pragma once
#include <functional>
#include <iostream>
#include <optional>

#include "base_algorithm.h"


template <typename SolutionClass>
class ea : public base_algorithm<SolutionClass>
{
private:
	solvable<SolutionClass>& problem_;
	std::function<SolutionClass(ea<SolutionClass>&, SolutionClass&, SolutionClass&)> crossover_;
	std::string crossover_operator_name_;
	std::function<SolutionClass(ea<SolutionClass>&, SolutionClass const&)> mutate_;
	std::string mutate_operator_name_;
	std::function<SolutionClass(ea<SolutionClass>&, std::vector<SolutionClass>&)> select_;
	std::string select_operator_name_;

	int pop_size_;
	int gen_;
	double px_;
	double pm_;
	int tour_;
public:
	ea(solvable<SolutionClass>& problem,
	   const std::function<SolutionClass(ea<SolutionClass>&, SolutionClass&, SolutionClass&)>& crossover,
	   const std::string& crossover_operator_name, const std::function<SolutionClass(ea<SolutionClass>&, SolutionClass const&)>& mutate,
	   const std::string& mutate_operator_name,
	   const std::function<SolutionClass(ea<SolutionClass>&, std::vector<SolutionClass>&)>& select,
	   const std::string& select_operator_name, const int pop_size, const int gen, const double px, const double pm,
	   const int tour)
		: problem_(problem),
		  crossover_(crossover),
		  crossover_operator_name_(crossover_operator_name),
		  mutate_(mutate),
		  mutate_operator_name_(mutate_operator_name),
		  select_(select),
		  select_operator_name_(select_operator_name),
		  pop_size_(pop_size),
		  gen_(gen),
		  px_(px),
		  pm_(pm),
		  tour_(tour)
	{
	}

	SolutionClass solve() override;


	[[nodiscard]] std::string crossover_operator_name() const
	{
		return crossover_operator_name_;
	}

	[[nodiscard]] std::string mutate_operator_name() const
	{
		return mutate_operator_name_;
	}

	[[nodiscard]] std::string select_operator_name() const
	{
		return select_operator_name_;
	}

	[[nodiscard]] int pop_size() const
	{
		return pop_size_;
	}

	[[nodiscard]] int gen() const
	{
		return gen_;
	}

	[[nodiscard]] double px() const
	{
		return px_;
	}

	[[nodiscard]] double pm() const
	{
		return pm_;
	}

	[[nodiscard]] int tour() const
	{
		return tour_;
	}

	[[nodiscard]] solvable<SolutionClass>& problem()
	{
		return problem_;
	}
};

template <typename SolutionClass>
class ea_generation_stats
{
private:
	ea<SolutionClass>& ea_;
	std::vector<SolutionClass>& generation_;
	double best_fitness_;
	double avg_fitness_;
	double worst_fitness_;
	std::optional<SolutionClass> best_solution_;
public:
	ea_generation_stats(ea<SolutionClass>& ea, std::vector<SolutionClass>& generation);

	void generate_stats();

	[[nodiscard]] double best_fitness() const
	{
		return best_fitness_;
	}

	[[nodiscard]] double avg_fitness() const
	{
		return avg_fitness_;
	}

	[[nodiscard]] double worst_fitness() const
	{
		return worst_fitness_;
	}

	[[nodiscard]] std::optional<SolutionClass> best_solution() const
	{
		return best_solution_;
	}
};

template <typename SolutionClass>
std::vector<SolutionClass> make_first_generation(ea<SolutionClass>& ea, int pop_size)
{
	std::vector<SolutionClass> sln;
	sln.reserve(pop_size);

	for (int i = 0; i < pop_size; i++)
	{
		sln.push_back(ea.problem().make_random_solution());
	}
	return sln;
}

template <typename SolutionClass>
ea_generation_stats<SolutionClass>::ea_generation_stats(ea<SolutionClass>& ea, std::vector<SolutionClass>& generation) :
	ea_(ea),
	generation_(generation)
{
	generate_stats();
}

template <typename SolutionClass>
void ea_generation_stats<SolutionClass>::generate_stats()
{
	const double init_fitness = this->ea_.problem().fitness(this->generation_.at(0));
	double best_fitness = init_fitness, worst_fitness = init_fitness;
	SolutionClass best_solution = this->generation_.at(0);
	double total_fitness = 0;

	for (auto& sln : this->generation_)
	{
		const double fitness = this->ea_.problem().fitness(sln);
		if (fitness > best_fitness)
		{
			best_fitness = fitness;
			best_solution = sln;
		}
		if (fitness < worst_fitness)
		{
			worst_fitness = fitness;
		}
		total_fitness += fitness;
	}

	best_fitness_ = best_fitness;
	best_solution_ = best_solution;
	worst_fitness_ = worst_fitness;
	avg_fitness_ = total_fitness / this->generation_.size();
}


template<typename SolutionClass>
inline SolutionClass ea<SolutionClass>::solve()
{
	int current_generation = 0;
	std::vector<SolutionClass> generation = make_first_generation(*this, this->pop_size());
	SolutionClass best_solution = generation.at(0);
	double best_solution_fitness = this->problem().fitness(generation.at(0));

	while (current_generation < this->gen())
	{
		// Remember best solution
		ea_generation_stats<SolutionClass> stats(*this, generation);
		if (stats.best_fitness() > best_solution_fitness)
		{
			if (stats.best_solution().has_value())
			{
				best_solution = stats.best_solution().value();
			}
			best_solution_fitness = stats.best_fitness();
		}

		// Start making new population
		std::vector<SolutionClass> new_generation;
		new_generation.reserve(generation.size());

		while (new_generation.size() < generation.size())
		{
			auto p1 = this->select_(*this, generation);
			auto p2 = this->select_(*this, generation);
			if ((rand() / (RAND_MAX + 1.)) < this->px())
			{
				new_generation.push_back(this->crossover_(*this, p1, p2));
			}
			else
			{
				new_generation.push_back(p1);
			}
		}
		std::cout << current_generation << ";" << stats.best_fitness() << ";" << stats.avg_fitness() << ";" << stats.worst_fitness() << std::endl;
		generation = new_generation;
		current_generation++;
	}

	return best_solution;
}