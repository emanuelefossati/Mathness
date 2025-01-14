#include "LnNode.h"

Result LnNode::GetResult() const
{
	Result childResult = _Child->GetResult();

	if (childResult.IsError())
		return childResult;

	if (!childResult.IsScalar())
		return Error("Ln function can only be applied to scalars", _TokenRange);

	scalar_t value = childResult.ToScalar();

	if(value <= 0)
		return Error("Ln function can only be applied to positive numbers", _TokenRange);

	return std::log(value);
}
