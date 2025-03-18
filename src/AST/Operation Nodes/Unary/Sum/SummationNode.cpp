#include "SummationNode.h"

EvaluationResult SummationNode::GetResult() const
{
	EvaluationResult childResult = _Child->GetResult();

	if (childResult.IsError() || childResult.IsScalar())
		return childResult;

	if (!childResult.IsList())
		return Error("Summation function can only be applied to lists", _TokenRange);

	auto childList = childResult.ToList();

	if (childList.Elements.size() == 0)
		return Error("Summation function cannot be applied to an empty list", _TokenRange);

	return childList.Sum();
}