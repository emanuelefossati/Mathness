#include "LnNode.h"

Result LnNode::GetResult() const
{
	Result childResult = _Child->GetResult();

	if (IsError(childResult))
		return childResult;

	if (!IsScalar(childResult))
		return error_t("Ln function can only be applied to scalars");

	scalar_t value = ResultToScalar(childResult);

	if(value <= 0)
		return error_t("Ln function can only be applied to positive numbers");

	return std::log(ResultToScalar(childResult));
}
