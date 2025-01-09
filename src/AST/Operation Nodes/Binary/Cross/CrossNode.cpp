#include "CrossNode.h"

Result CrossNode::GetResult() const override
{
	auto leftResult = _Left->GetResult();
	auto rightResult = _Right->GetResult();

	if (std::holds_alternative<error_t>(leftResult))
		return leftResult;

	if (std::holds_alternative<error_t>(rightResult))
		return rightResult;

	if (!IsMatrix(leftResult) || !IsMatrix(rightResult))
		return error_t("cross product is only defined for vectors");

	Matrix leftMatrix = std::get<Matrix>(leftResult);
	Matrix rightMatrix = std::get<Matrix>(rightResult);

	if (!leftMatrix.IsVector() || !rightMatrix.IsVector())
		return error_t("cross product is only defined for vectors");

	if (leftMatrix.Rows  != 3 || rightMatrix.Rows != 3)
		return error_t("cross product is only defined for vectors of size 3")

	return leftMatrix.Cross(rightMatrix);
}