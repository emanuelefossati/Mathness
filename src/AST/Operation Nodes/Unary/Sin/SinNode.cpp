#include "SinNode.h"

Result SinNode::GetResult() const
{
	Result childResult = _Child->GetResult();

	if (IsError(childResult))
		return childResult;

	if (!IsScalar(childResult))
		return error_t("Sin function can only be applied to scalars");

	return std::sin(ResultToScalar(childResult));
}