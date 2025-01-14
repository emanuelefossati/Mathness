#include "ArccosNode.h"

Result ArccosNode::GetResult() const
{
	Result childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return Error("Arccos function can only be applied to scalars", _TokenRange);

	return std::acos(childResult.ToScalar());
}