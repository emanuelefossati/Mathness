#include "LogNode.h"

Result LogNode::GetResult() const
{
	auto left = _Left->GetResult();
	auto right = _Right->GetResult();

	if (std::holds_alternative<error_t>(left))
		return left;

	if (std::holds_alternative<error_t>(right))
		return right;

	if (!IsScalar(left) || !IsScalar(right))
		return error_t("cannot perform log operation on non-scalar types");

	scalar_t base = std::get<scalar_t>(left);
	scalar_t argument = std::get<scalar_t>(right);

	if (argument == 0)
		return error_t("cannot perform log operation if the argument is 0");

	if (base == 1)
		return error_t("cannot perform log operation if the base is 1");

	scalar_t result = std::log(argument) / std::log(base);

	return result;
}
