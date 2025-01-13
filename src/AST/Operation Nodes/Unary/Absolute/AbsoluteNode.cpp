#include "AbsoluteNode.h"

Result AbsoluteNode::GetResult() const
{
	auto childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if(childResult.IsList())
		return error_t("Cannot take the absolute value of a list");

	if (childResult.IsScalar())
		return abs(childResult.ToScalar());

	if (childResult.IsMatrix())
		return childResult.ToMatrix().Norm();
}