#include "SqrtNode.h"

Result SqrtNode::GetResult() const
{
	Result result = _Child->GetResult();

	if (result.IsError())
		return result;

	if (!result.IsScalar())
		return error_t("Sqrt function can only be applied to scalars");

	scalar_t value = result.ToScalar();

	if (value < 0)
		return error_t("Sqrt function can only be applied to non-negative scalars");

	return std::sqrt(value);
}