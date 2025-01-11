#include  "ModulusNode.h"

Result ModulusNode::GetResult() const
{
	auto left = _Left->GetResult();
	auto right = _Right->GetResult();

	if (IsError(left))
		return left;

	if (IsError(right))
		return right;

	if (!IsScalar(left) || !IsScalar(right))
		return error_t("cannot perform modulus operation on non-scalar types");

	if (!IsScalarInteger(ResultToScalar(left)) || !IsScalarInteger(ResultToScalar(right)))
		return error_t("cannot perform modulus operation on non-integer scalars");

	scalar_t result = lround(ResultToScalar(left)) % lround(ResultToScalar(right));
	return result;

}