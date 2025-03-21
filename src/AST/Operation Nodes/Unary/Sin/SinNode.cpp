#include "SinNode.h"

EvaluationResult SinNode::GetResult() const
{
	EvaluationResult childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return Error("Sin function can only be applied to scalars", _TokenRange);

	scalar_t argument = childResult.ToScalar();

	// Normalize the angle in the range [0, 2pi)
	argument = NormalizeAngle(argument);

	if (double_equals(argument, 0) || double_equals(argument, std::numbers::pi))
		return scalar_t{ 0 };

	if(double_equals(argument, std::numbers::pi/2))
		return scalar_t{ 1 };

	if (double_equals(argument, 3 * std::numbers::pi / 2))
		return scalar_t{ -1 };

	return std::sin(argument);
}