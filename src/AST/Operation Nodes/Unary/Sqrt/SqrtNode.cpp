#include "SqrtNode.h"

Result SqrtNode::GetResult() const
{
	Result result = _Child->GetResult();

	if (IsError(result))
		return result;

	if (!IsScalar(result))
		return error_t("Sqrt function can only be applied to scalars");

	scalar_t value = ResultToScalar(result);

	if (value < 0)
		return error_t("Sqrt function can only be applied to non-negative scalars");

	return std::sqrt(value);
}