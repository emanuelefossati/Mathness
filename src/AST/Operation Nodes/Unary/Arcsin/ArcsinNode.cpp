#include "ArcsinNode.h"

Result ArcsinNode::GetResult() const
{
	Result childResult = _Child->GetResult();

	if (IsError(childResult))
		return childResult;

	if (!IsScalar(childResult))
		return error_t("Arcsin function can only be applied to scalars");

	return std::asin(ResultToScalar(childResult));
}