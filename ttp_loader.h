#pragma once
#include <stdexcept>
#include <string>

#include "ttp.h"


class ttp_loader_error final : public std::runtime_error
{
public:
	explicit ttp_loader_error(const std::string& message)
		: runtime_error(message)
	{
	}

	explicit ttp_loader_error(const char* const message)
		: runtime_error(message)
	{
	}
};


class ttp_loader
{
private:
	std::string filename_;
	
	static std::tuple<std::string, std::string> parse_line(const std::string& line);
	static ttp_node get_node_value(const std::string& line);
	static ttp_item get_item_value(const std::string& line);
public:
	explicit ttp_loader(std::string filename)
		: filename_(std::move(filename))
	{
	}

	[[nodiscard]] std::string filename() const
	{
		return filename_;
	}

	[[nodiscard]] ttp load() const;
};
