#include "DistinctNode.h"

EvaluationResult DistinctNode::GetResult() const
{
	EvaluationResult childResult = child->GetResult();

	if(childResult.IsError() || childResult.IsScalar())
		return childResult;

	if (!childResult.IsList())
		return Error("Distinct function can only be applied to lists");

	if (childResult.ToList().Elements.empty())
		return Error("Distinct function cannot be applied to an empty list");

	auto list = childResult.ToList();

	return list.Distinct();
}