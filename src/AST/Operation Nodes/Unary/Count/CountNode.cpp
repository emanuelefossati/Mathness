#include "CountNode.h"

EvaluationResult CountNode::GetResult() const
{
	EvaluationResult childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsMatrix())
		return Error("Count function can only be applied to matrices", _TokenRange);

	auto list = childResult.ToList();

	return list.Count();
}