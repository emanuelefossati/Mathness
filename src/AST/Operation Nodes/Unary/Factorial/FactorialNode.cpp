#include "FactorialNode.h"

EvaluationResult FactorialNode::GetResult() const
{
	EvaluationResult childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return Error("Factorial function can only be applied to scalars", _TokenRange);

	scalar_t value = childResult.ToScalar();

	if (value < 0 || EvaluationResult::IsScalarInteger(value))
		return Error("Factorial function can only be applied to non-negative integers", _TokenRange);

	return Factorial(value);
}

scalar_t FactorialNode::Factorial(scalar_t n) const
{
	for (scalar_t i = n - 1; i > 1; i--)
		n *= i;

	return n;
}