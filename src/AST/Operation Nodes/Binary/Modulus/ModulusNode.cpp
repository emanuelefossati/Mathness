#include  "ModulusNode.h"

EvaluationResult ModulusNode::GetResult() const
{
	auto left = _Left->GetResult();
	auto right = _Right->GetResult();

	if (left.IsError())
		return left;

	if (right.IsError())
		return right;

	if (!left.IsScalar() || !right.IsScalar())
		return Error("cannot perform modulus operation on non-scalar types");

	if (!left.IsScalarInteger() || !right.IsScalarInteger())
		return Error("cannot perform modulus operation on non-integer scalars");

	scalar_t result = lround(left.ToScalar()) % lround(right.ToScalar());
	return result;

}