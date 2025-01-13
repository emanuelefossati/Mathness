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
		return error_t("cross product is only defined for vectors");

	Matrix leftMatrix = leftResult.ToMatrix();
	Matrix rightMatrix = rightResult.ToMatrix();

	if (!leftMatrix.IsVector() || !rightMatrix.IsVector())
		return error_t("cross product is only defined for vectors");

	if (leftMatrix.Rows != 3 || rightMatrix.Rows != 3)
		return error_t("cross product is only defined for vectors of size 3");

	return leftMatrix.Cross(rightMatrix);
}