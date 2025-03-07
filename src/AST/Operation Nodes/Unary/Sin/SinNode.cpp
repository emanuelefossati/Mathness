#include "SinNode.h"

EvaluationResult SinNode::GetResult() const
{
	EvaluationResult childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return Error("Sin function can only be applied to scalars", _TokenRange);

	auto childResultValue = childResult.ToScalar();

	return std::sin(childResultValue);
}