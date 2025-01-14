#pragma once

#include <string>
#include <cmath>


using error_t = std::string;

struct Range
{
	size_t Start = -1;
	size_t Length = 0;
};

struct Error : std::string
{
	using std::string::string;

	Error(std::string message) : std::string(message) {}
	Error(std::string message, Range range) : std::string(message), Range(range) {}

	Range Range;
};

using scalar_t = double;
