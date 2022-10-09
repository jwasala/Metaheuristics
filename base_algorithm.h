#pragma once

template <typename SolutionClass>
class solvable
{
public:
	virtual SolutionClass make_random_solution() = 0;
	virtual double fitness(SolutionClass& solution) = 0;
};

template <typename SolutionClass>
class base_algorithm
{
public:
	virtual SolutionClass solve() = 0;
};
