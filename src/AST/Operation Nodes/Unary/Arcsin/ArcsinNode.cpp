#include "ArcsinNode.h"

EvaluationResult ArcsinNode::GetResult() const
{
	EvaluationResult childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return Error("Arcsin function can only be applied to scalars");

	return std::asin(childResult.ToScalar());
}