#include "AbsoluteNode.h"

Result AbsoluteNode::GetResult() const
{
	auto childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if(childResult.IsList())
		return Error("Cannot take the absolute value of a list", _TokenRange);

	if (childResult.IsScalar())
		return abs(childResult.ToScalar());

	
	return childResult.ToMatrix().Norm();
}