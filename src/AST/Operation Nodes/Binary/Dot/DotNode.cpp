#include "DotNode.h"

Result DotNode::GetResult() const
{
	auto left = _Left->GetResult();
	auto right = _Right->GetResult();

	if (IsError(left))
		return left;

	if (IsError(right))
		return right;

	if (!IsMatrix(left) || !IsMatrix(right))
		return error_t("cannot perform dot operation on non-vector types");

	Matrix leftMatrix = ResultToMatrix(left);
	Matrix rightMatrix = ResultToMatrix(right);

	if (!leftMatrix.IsVector() || !rightMatrix.IsVector())
		return error_t("cannot perform dot operation on non-vector types");

	if (leftMatrix.Elements.size() != rightMatrix.Elements.size())
		return error_t("cannot perform dot operation on vectors of different sizes");

	return leftMatrix.Dot(rightMatrix);
}