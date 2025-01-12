#include "ExpNode.h"

Result ExpNode::GetResult() const
{
	Result childResult = _Child->GetResult();

	if (IsError(childResult))
		return childResult;

	if (!IsScalar(childResult))
		return error_t("Exponential function can only be applied to scalars");

	return std::exp(ResultToScalar(childResult));
}