#include "TanNode.h"

Result TanNode::GetResult() const
{
	Result childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return error_t("Tan function can only be applied to scalars");

	scalar_t value = childResult.ToScalar();

	if (std::cos(value) == 0)
		return error_t("Tan function is undefined at this point");

	return std::tan(value);
}