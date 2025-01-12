#include "ArctanNode.h"

Result ArctanNode::GetResult() const
{
	Result childResult = _Child->GetResult();

	if (IsError(childResult))
		return childResult;

	if (!IsScalar(childResult))
		return error_t("Arctan function can only be applied to scalars");

	return std::atan(ResultToScalar(childResult));
}