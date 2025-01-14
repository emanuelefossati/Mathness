#include "ArctanNode.h"

Result ArctanNode::GetResult() const
{
	Result childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return Error("Arctan function can only be applied to scalars", _TokenRange);

	return std::atan(childResult.ToScalar());
}