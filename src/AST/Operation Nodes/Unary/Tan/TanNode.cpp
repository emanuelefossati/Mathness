#include "TanNode.h"

EvaluationResult TanNode::GetResult() const
{
	EvaluationResult childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return Error("Tan function can only be applied to scalars", _TokenRange);

	scalar_t value = childResult.ToScalar();

	if (std::cos(value) == 0)
		return Error("Tan function is undefined at this point", _TokenRange);

	return std::tan(value);
}