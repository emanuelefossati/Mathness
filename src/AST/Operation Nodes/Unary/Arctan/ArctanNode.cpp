#include "ArctanNode.h"

EvaluationResult ArctanNode::GetResult() const
{
	EvaluationResult childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return Error("Arctan function can only be applied to scalars");

	return std::atan(childResult.ToScalar());
}