#include "CountNode.h"

EvaluationResult CountNode::GetResult() const
{
	EvaluationResult childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsList())
		return Error("Count function can only be applied to lists");

	auto list = childResult.ToList();

	return list.Count();
}