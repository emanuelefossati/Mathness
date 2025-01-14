#include "CrossNode.h"

Result CrossNode::GetResult() const
{
	auto leftResult = _Left->GetResult();
	auto rightResult = _Right->GetResult();

	if (leftResult.IsError())
		return leftResult;

	if (rightResult.IsError())
		return rightResult;

	if (!leftResult.IsMatrix() || !rightResult.IsMatrix())
		return Error("cross product is only defined for vectors", _TokenRange);

	Matrix leftMatrix = leftResult.ToMatrix();
	Matrix rightMatrix = rightResult.ToMatrix();

	if (!leftMatrix.IsVector() || !rightMatrix.IsVector())
		return Error("cross product is only defined for vectors", _TokenRange);

	if (leftMatrix.Rows != 3 || rightMatrix.Rows != 3)
		return Error("cross product is only defined for vectors of size 3", _TokenRange);

	return leftMatrix.Cross(rightMatrix);
}