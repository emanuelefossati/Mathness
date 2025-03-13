#pragma once

#include <string>
#include <cmath>
#include <numbers>
#include <fmt/core.h>


using error_t = std::string;

struct Range
{
	Range(size_t start, size_t length) : Start(start), Length(length) {}
	Range(size_t start): Start(start), Length(1) {}
	Range() = default;

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


static bool double_equals(double a, double b)
{
	return std::abs(a - b) < std::numeric_limits<double>::min();
}