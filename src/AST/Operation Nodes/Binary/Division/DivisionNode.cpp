#include "DivisionNode.h"

Result DivisionNode::GetResult() const
{
	auto leftResult = _Left->GetResult();
	auto rightResult = _Right->GetResult();

	if (std::holds_alternative<error_t>(leftResult))
		return leftResult;

	if (std::holds_alternative<error_t>(rightResult))	
		return rightResult;

	if (std::holds_alternative<List>(leftResult) || std::holds_alternative<List>(rightResult))
		return error_t("Cannot perform division with lists");

	if (AreBothScalars(leftResult, rightResult))
	{
		scalar_t leftScalar = std::get<scalar_t>(leftResult);
		scalar_t rightScalar = std::get<scalar_t>(rightResult);

		if (rightScalar == 0)
			return error_t("Division by zero");

		return leftScalar / rightScalar;
	}

	if (!IsMatrix(rightResult))
	{
		scalar_t rightScalar = std::get<scalar_t>(rightResult);
		Matrix leftMatrix = std::get<Matrix>(leftResult);

		if (rightScalar == 0)
			return error_t("Division by zero");

		return leftMatrix / rightScalar;
	}

	Matrix rightMatrix = std::get<Matrix>(rightResult);

	if(!rightMatrix.IsSquare())
		return error_t("Cannot perform division because divisor matrix is not square");

	if (rightMatrix.IsSingular())
		return error_t("Cannot perform division because divisor matrix is singular");
	

	if (AreBothMatrices(leftResult, rightResult))
	{
		return GetMatrixDivision(std::get<Matrix>(leftResult), std::get<Matrix>(rightResult));
	}


	return std::get<scalar_t>(leftResult) * rightMatrix.Inverse();
	
}

Result DivisionNode::GetMatrixDivision(const Matrix& left, const Matrix& right) const
{
	if (!left.IsSquare())
		return error_t("Cannot perform division because dividend matrix is singular");

	if (left.Rows != right.Rows)
		return error_t("Both matrices must have the same size to perform division");

	return left * right.Inverse();
}