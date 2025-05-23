#include "ExpNode.h"

EvaluationResult ExpNode::GetResult() const
{
	EvaluationResult childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return Error("Exponential function can only be applied to scalars");

	return std::exp(childResult.ToScalar());
}