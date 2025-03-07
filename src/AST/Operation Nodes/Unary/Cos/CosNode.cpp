#include "CosNode.h"


EvaluationResult CosNode::GetResult() const
{
	EvaluationResult childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return Error("Cosin function can only be applied to scalars", _TokenRange);

	auto childResultValue = childResult.ToScalar();

	return std::cos(childResultValue);
}