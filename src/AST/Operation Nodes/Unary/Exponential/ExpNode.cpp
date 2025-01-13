#include "ExpNode.h"

Result ExpNode::GetResult() const
{
	Result childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return error_t("Exponential function can only be applied to scalars");

	return std::exp(childResult.ToScalar());
}