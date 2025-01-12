#include "FactorialNode.h"

Result FactorialNode::GetResult() const
{
	Result childResult = _Child->GetResult();

	if (IsError(childResult))
		return childResult;

	if (!IsScalar(childResult))
		return error_t("Factorial function can only be applied to scalars");

	scalar_t value = ResultToScalar(childResult);

	if (value < 0 || std::floor(value) != value)
		return error_t("Factorial function can only be applied to non-negative integers");

	return Factorial(value);
}

scalar_t Factorial(scalar_t n)
{
	for (scalar_t i = n - 1; i > 1; i--)
		n *= i;

	return n;
}