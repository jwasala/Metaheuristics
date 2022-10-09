#pragma once
#include <string>

#include "config.h"

class config_loader
{
private:
	std::string filename_;
public:
	explicit config_loader(std::string filename)
		: filename_(std::move(filename))
	{
	}

	[[nodiscard]] config load() const;

	[[nodiscard]] std::string filename() const
	{
		return filename_;
	}
};
