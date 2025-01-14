#include "ExpNode.h"

Result ExpNode::GetResult() const
{
	Result childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return Error("Exponential function can only be applied to scalars", _TokenRange);

	return std::exp(childResult.ToScalar());
}