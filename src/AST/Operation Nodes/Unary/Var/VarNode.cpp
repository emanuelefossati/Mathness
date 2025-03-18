#include "VarNode.h"

EvaluationResult VarNode::GetResult() const
{
	EvaluationResult childResult = _Child->GetResult();

	if(childResult.IsError() || childResult.IsScalar())
		return childResult;

	if (!childResult.IsList())
		return Error("Variance function can only be applied to lists");

	auto list = childResult.ToList();

	if (list.Elements.empty())
		return Error("Variance function can only be applied to non-empty lists");

	return list.Variance();


}