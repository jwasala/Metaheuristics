#pragma once
#include <string>

class config
{
private:
	std::string problem_files_directory_;
public:
	explicit config(std::string problem_files_directory)
		: problem_files_directory_(std::move(problem_files_directory))
	{
	}

	[[nodiscard]] std::string problem_files_directory() const
	{
		return problem_files_directory_;
	}
};
