#include "SinNode.h"

EvaluationResult SinNode::GetResult() const
{
	EvaluationResult childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return Error("Sin function can only be applied to scalars", _TokenRange);

	return std::sin(childResult.ToScalar());
}