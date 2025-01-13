#include "LnNode.h"

Result LnNode::GetResult() const
{
	Result childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return error_t("Ln function can only be applied to scalars");

	scalar_t value = childResult.ToScalar();

	if(value <= 0)
		return error_t("Ln function can only be applied to positive numbers");

	return std::log(value);
}
