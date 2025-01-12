#include "AbsoluteNode.h"

Result AbsoluteNode::GetResult() const
{
	auto childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if(IsList(childResult))
		return error_t("Cannot take the absolute value of a list");

	if (IsScalar(childResult))
		return abs(ResultToScalar(childResult));

	if (IsMatrix(childResult))
		return ResultToMatrix(childResult).Norm();
}