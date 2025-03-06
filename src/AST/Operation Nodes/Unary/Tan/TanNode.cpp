#include "TanNode.h"

EvaluationResult TanNode::GetResult() const
{
	EvaluationResult childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return Error("Tan function can only be applied to scalars", _TokenRange);

	scalar_t value = childResult.ToScalar();

	if (double_equals(std::fmod(value, std::numbers::pi), std::numbers::pi / 2))
		return Error("Tan function is undefined at this point", _TokenRange);

	if (double_equals(std::fmod(value, std::numbers::pi), std::numbers::pi))
		return scalar_t{ 0 };

	if(double_equals(std::fmod(value, std::numbers::pi), std::numbers::pi/4))
		return scalar_t{ 1 };

	if (double_equals(std::fmod(value, std::numbers::pi), std::numbers::pi * 3 / 4))
		return scalar_t{ -1 };

	return std::tan(value);
}