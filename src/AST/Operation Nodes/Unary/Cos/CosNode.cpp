#include "CosNode.h"

Result CosNode::GetResult() const
{
	Result childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return error_t("Cosin function can only be applied to scalars");

	return std::cos(childResult.ToScalar());
}