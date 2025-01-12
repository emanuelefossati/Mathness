#include "ArccosNode.h"

Result ArccosNode::GetResult() const
{
	Result childResult = _Child->GetResult();

	if (IsError(childResult))
		return childResult;

	if (!IsScalar(childResult))
		return error_t("Arccos function can only be applied to scalars");

	return std::acos(ResultToScalar(childResult));
}