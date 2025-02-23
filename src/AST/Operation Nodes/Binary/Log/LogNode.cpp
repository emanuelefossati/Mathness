#include "LogNode.h"

EvaluationResult LogNode::GetResult() const
{
	auto left = _Left->GetResult();
	auto right = _Right->GetResult();

	if (left.IsError())
		return left;

	if (right.IsError())
		return right;

	if (!left.IsScalar() || !right.IsScalar())
		return Error("cannot perform log operation on non-scalar types", _TokenRange);

	scalar_t base = left.ToScalar();
	scalar_t argument = right.ToScalar();

	if (argument == 0)
		return Error("cannot perform log operation if the argument is 0", _TokenRange);

	if (base == 1)
		return Error("cannot perform log operation if the base is 1", _TokenRange);

	scalar_t result = std::log(argument) / std::log(base);

	return result;
}
