#include "CrossNode.h"

Result CrossNode::GetResult() const
{
	auto leftResult = _Left->GetResult();
	auto rightResult = _Right->GetResult();

	if (IsError(leftResult))
		return leftResult;

	if (IsError(rightResult))
		return rightResult;

	if (!IsMatrix(leftResult) || !IsMatrix(rightResult))
		return error_t("cross product is only defined for vectors");

	Matrix leftMatrix = ResultToMatrix(leftResult);
	Matrix rightMatrix = ResultToMatrix(rightResult);

	if (!leftMatrix.IsVector() || !rightMatrix.IsVector())
		return error_t("cross product is only defined for vectors");

	if (leftMatrix.Rows != 3 || rightMatrix.Rows != 3)
		return error_t("cross product is only defined for vectors of size 3");

	return leftMatrix.Cross(rightMatrix);
}