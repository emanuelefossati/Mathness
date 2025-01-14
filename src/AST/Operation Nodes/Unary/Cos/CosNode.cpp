#include "CosNode.h"

Result CosNode::GetResult() const
{
	Result childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return Error("Cosin function can only be applied to scalars", _TokenRange);

	return std::cos(childResult.ToScalar());
}