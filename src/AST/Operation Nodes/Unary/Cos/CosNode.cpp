#include "CosNode.h"

Result CosNode::GetResult() const
{
	Result childResult = _Child->GetResult();

	if (IsError(childResult))
		return childResult;

	if (!IsScalar(childResult))
		return error_t("Cosin function can only be applied to scalars");

	return std::cos(ResultToScalar(childResult));
}