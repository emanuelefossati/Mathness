#include "ArctanNode.h"

Result ArctanNode::GetResult() const
{
	Result childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return error_t("Arctan function can only be applied to scalars");

	return std::atan(childResult.ToScalar());
}