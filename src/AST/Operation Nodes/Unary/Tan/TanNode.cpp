#include "TanNode.h"

EvaluationResult TanNode::GetResult() const
{
	EvaluationResult childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return Error("Tan function can only be applied to scalars", _TokenRange);

	scalar_t value = childResult.ToScalar();

	return std::tan(value);
}