#include "TanNode.h"

Result TanNode::GetResult() const
{
	Result childResult = _Child->GetResult();

	if (IsError(childResult))
		return childResult;

	if (!IsScalar(childResult))
		return error_t("Tan function can only be applied to scalars");

	scalar_t value = ResultToScalar(childResult);

	if (std::cos(value) == 0)
		return error_t("Tan function is undefined at this point");

	return std::tan(value);
}

