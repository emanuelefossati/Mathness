#pragma once

#include <string>
#include <cmath>
#include <numbers>
#include <fmt/core.h>


using error_t = std::string;

struct Error : std::string
{
	using std::string::string;

	Error(std::string message) : std::string(message) {}
};

using scalar_t = double;


static bool double_equals(double a, double b)
{
	return std::abs(a - b) < std::numeric_limits<double>::min();
}

static scalar_t NormalizeAngle(scalar_t angle)
{
	angle = std::fmod(angle, 2 * std::numbers::pi);
	if (angle < 0)
		angle += 2 * std::numbers::pi;

	return angle;
}