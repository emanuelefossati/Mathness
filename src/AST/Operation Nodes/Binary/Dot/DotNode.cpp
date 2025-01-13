#include "DotNode.h"

Result DotNode::GetResult() const
{
	auto left = _Left->GetResult();
	auto right = _Right->GetResult();

	if (left.IsError())
		return left;

	if (right.IsError())
		return right;

	if (!left.IsMatrix() || !right.IsMatrix())
		return error_t("cannot perform dot operation on non-vector types");

	Matrix leftMatrix = left.ToMatrix();
	Matrix rightMatrix = right.ToMatrix();

	if (!leftMatrix.IsVector() || !rightMatrix.IsVector())
		return error_t("cannot perform dot operation on non-vector types");

	if (leftMatrix.Elements.size() != rightMatrix.Elements.size())
		return error_t("cannot perform dot operation on vectors of different sizes");

	return leftMatrix.Dot(rightMatrix);
}