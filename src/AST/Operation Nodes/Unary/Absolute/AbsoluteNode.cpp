#include "AbsoluteNode.h"

EvaluationResult AbsoluteNode::GetResult() const
{
	auto childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if(childResult.IsList())
		return Error("Cannot take the absolute value of a list");

	if (childResult.IsScalar())
		return abs(childResult.ToScalar());

	
	return childResult.ToMatrix().Norm();
}