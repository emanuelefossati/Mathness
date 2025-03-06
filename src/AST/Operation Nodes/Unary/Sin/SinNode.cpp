#include "SinNode.h"

EvaluationResult SinNode::GetResult() const
{
	EvaluationResult childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return Error("Sin function can only be applied to scalars", _TokenRange);

	//Well known results

	auto childResultValue = childResult.ToScalar();

	if (double_equals(std::fmod(childResultValue, std::numbers::pi), 0))
		return scalar_t{ 0 };

	if (double_equals(std::fmod(childResultValue, 2 * std::numbers::pi), std::numbers::pi/2))
		return scalar_t{ 1 };

	if (double_equals(std::fmod(childResultValue, 2 * std::numbers::pi), std::numbers::pi * 3 / 2))
		return scalar_t{ -1 };

	return std::sin(childResultValue);
}