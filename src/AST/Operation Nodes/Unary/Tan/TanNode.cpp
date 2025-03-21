#include "TanNode.h"

EvaluationResult TanNode::GetResult() const
{
	EvaluationResult childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return Error("Tan function can only be applied to scalars", _TokenRange);

	scalar_t argument = childResult.ToScalar();

	argument = NormalizeAngle(argument);

	if (double_equals(argument, std::numbers::pi / 2))
		return Error("Tan function is not defined for angles of pi/2");

	if (double_equals(argument, 3 * std::numbers::pi / 2))
		return Error("Tan function is not defined for angles of 3/2pi");

	if (double_equals(argument, 0) || double_equals(argument, std::numbers::pi))
		return scalar_t{ 0 };

	if (double_equals(argument, std::numbers::pi/4) || double_equals(argument, 5 * std::numbers::pi / 4 ))
		return scalar_t{ 1 };

	if (double_equals(argument, 3 * std::numbers::pi / 4) || double_equals(argument, 7 * std::numbers::pi / 4))
		return scalar_t{ -1 };

	return std::tan(argument);
}