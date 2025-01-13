#include "FactorialNode.h"

Result FactorialNode::GetResult() const
{
	Result childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return error_t("Factorial function can only be applied to scalars");

	scalar_t value = childResult.ToScalar();

	if (value < 0 || Result::IsScalarInteger(value))
		return error_t("Factorial function can only be applied to non-negative integers");

	return Factorial(value);
}

scalar_t FactorialNode::Factorial(scalar_t n) const
{
	for (scalar_t i = n - 1; i > 1; i--)
		n *= i;

	return n;
}