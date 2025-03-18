#include "AvgNode.h"

EvaluationResult AvgNode::GetResult() const
{
	EvaluationResult childResult = _Child->GetResult();

	if (childResult.IsError() || childResult.IsScalar())
		return childResult;

	if (!childResult.IsList())
		return Error("Average function can only be applied to lists", _TokenRange);

	auto childList = childResult.ToList();

	if (childList.Elements.size() == 0)
		return Error("Average function cannot be applied to an empty list", _TokenRange);

	return childList.Average();
}