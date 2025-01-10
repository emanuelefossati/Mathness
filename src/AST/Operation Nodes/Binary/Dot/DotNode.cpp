#include "DotNode.h"

Result DotNode::GetResult() const
{
	auto left = _Left->GetResult();
	auto right = _Right->GetResult();

	if (std::holds_alternative<error_t>(left))
		return left;

	if (std::holds_alternative<error_t>(right))
		return right;

	if (!IsMatrix(left) || !IsMatrix(right))
		return error_t("cannot perform dot operation on non-vector types");

	Matrix leftMatrix = std::get<Matrix>(left);
	Matrix rightMatrix = std::get<Matrix>(right);

	if (!leftMatrix.IsVector() || !rightMatrix.IsVector())
		return error_t("cannot perform dot operation on non-vector types");

	if (leftMatrix.Elements.size() != rightMatrix.Elements.size())
		return error_t("cannot perform dot operation on vectors of different sizes");

	return leftMatrix.Dot(rightMatrix);
}