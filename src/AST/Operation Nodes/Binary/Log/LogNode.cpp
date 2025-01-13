#include "LogNode.h"

Result LogNode::GetResult() const
{
	auto left = _Left->GetResult();
	auto right = _Right->GetResult();

	if (left.IsError())
		return left;

	if (right.IsError())
		return right;

	if (!left.IsScalar() || !right.IsScalar())
		return error_t("cannot perform log operation on non-scalar types");

	scalar_t base = left.ToScalar();
	scalar_t argument = right.ToScalar();

	if (argument == 0)
		return error_t("cannot perform log operation if the argument is 0");

	if (base == 1)
		return error_t("cannot perform log operation if the base is 1");

	scalar_t result = std::log(argument) / std::log(base);

	return result;
}
