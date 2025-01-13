#include "ArcsinNode.h"

Result ArcsinNode::GetResult() const
{
	Result childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return error_t("Arcsin function can only be applied to scalars");

	return std::asin(childResult.ToScalar());
}