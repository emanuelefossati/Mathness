#include "LnNode.h"

EvaluationResult LnNode::GetResult() const
{
	EvaluationResult childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return Error("Ln function can only be applied to scalars");

	scalar_t value = childResult.ToScalar();

	if(value <= 0)
		return Error("Ln function can only be applied to positive numbers");

	return std::log(value);
}
