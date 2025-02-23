#include "SqrtNode.h"

EvaluationResult SqrtNode::GetResult() const
{
	EvaluationResult result = _Child->GetResult();

	if (result.IsError())
		return result;

	if (!result.IsScalar())
		return Error("Sqrt function can only be applied to scalars", _TokenRange);

	scalar_t value = result.ToScalar();

	if (value < 0)
		return Error("Sqrt function can only be applied to non-negative scalars", _TokenRange);

	return std::sqrt(value);
}