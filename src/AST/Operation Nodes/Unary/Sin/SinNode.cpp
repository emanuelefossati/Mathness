#include "SinNode.h"

Result SinNode::GetResult() const
{
	Result childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return error_t("Sin function can only be applied to scalars");

	return std::sin(childResult.ToScalar());
}