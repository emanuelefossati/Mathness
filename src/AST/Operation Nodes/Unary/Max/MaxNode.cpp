#include "MaxNode.h"

EvaluationResult MaxNode::GetResult() const
{
	EvaluationResult childResult = _Child->GetResult();

	if (childResult.IsError() || childResult.IsScalar())
		return childResult;

	if (!childResult.IsList())
		return Error("Max function can only be applied to lists");

	auto childList = childResult.ToList();

	if (childList.Elements.size() == 0)
		return Error("Max function cannot be applied to an empty list");

	return childList.Max();
}