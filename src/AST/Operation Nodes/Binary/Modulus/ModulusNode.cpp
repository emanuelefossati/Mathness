#include  "ModulusNode.h"

Result ModulusNode::GetResult() const
{
	auto left = _Left->GetResult();
	auto right = _Right->GetResult();

	if (std::holds_alternative<error_t>(left))
		return left;

	if (std::holds_alternative<error_t>(right))
		return right;

	if (!IsScalar(left) || !IsScalar(right))
		return error_t("cannot perform modulus operation on non-scalar types");

	return std::get<scalar_t>(left) % std::get<scalar_t>(right);

}