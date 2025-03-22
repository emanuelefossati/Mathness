#include "CrossNode.h"

EvaluationResult CrossNode::GetResult() const
{
	auto leftResult = _Left->GetResult();
	auto rightResult = _Right->GetResult();

	if (leftResult.IsError())
		return leftResult;

	if (rightResult.IsError())
		return rightResult;

	if (!leftResult.IsMatrix() || !rightResult.IsMatrix())
		return Error("cross product is only defined for vectors");

	Matrix leftMatrix = leftResult.ToMatrix();
	Matrix rightMatrix = rightResult.ToMatrix();

	if (!leftMatrix.IsVector() || !rightMatrix.IsVector())
		return Error("cross product is only defined for vectors");

	if (leftMatrix.Elements.size() != 3 || rightMatrix.Elements.size() != 3)
		return Error("cross product is only defined for vectors of size 3");

	return leftMatrix.Cross(rightMatrix);
}