#include "ArccosNode.h"

EvaluationResult ArccosNode::GetResult() const
{
	EvaluationResult childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return Error("Arccos function can only be applied to scalars");

	return std::acos(childResult.ToScalar());
}