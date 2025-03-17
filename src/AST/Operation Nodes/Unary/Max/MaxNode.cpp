#include "MaxNode.h"

EvaluationResult MaxNode::GetResult() const
{
	EvaluationResult childResult = _Child->GetResult();

	if (childResult.IsError() || childResult.IsScalar())
		return childResult;

	if (!childResult.IsList())
		return Error("Max function can only be applied to lists", _TokenRange);

	auto childList = childResult.ToList();

	if (childResultValue.empty())
		return Error("Max function cannot be applied to an empty list", _TokenRange);

	auto max = childList.Elements[0].Value;

	for (const auto& element : childList.Elements)
		if (element.Value > max)
			max = element.Value;

	return max;
}