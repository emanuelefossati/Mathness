#include "ArccosNode.h"

Result ArccosNode::GetResult() const
{
	Result childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return error_t("Arccos function can only be applied to scalars");

	return std::acos(childResult.ToScalar());
}