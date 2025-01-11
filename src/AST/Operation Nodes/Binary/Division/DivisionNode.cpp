#include "DivisionNode.h"

Result DivisionNode::GetResult() const
{
	auto leftResult = _Left->GetResult();
	auto rightResult = _Right->GetResult();

	if (IsError(leftResult))
		return leftResult;

	if (IsError(rightResult))	
		return rightResult;

	if (IsList(leftResult) || IsList(rightResult))
		return error_t("Cannot perform division with lists");

	if (IsScalar(leftResult) && IsScalar(rightResult))
	{
		scalar_t leftScalar = ResultToScalar(leftResult);
		scalar_t rightScalar = ResultToScalar(rightResult);

		if (rightScalar == 0)
			return error_t("Division by zero");

		return leftScalar / rightScalar;
	}

	if (!IsMatrix(rightResult))
	{
		scalar_t rightScalar = ResultToScalar(rightResult);
		Matrix leftMatrix = ResultToMatrix(leftResult);

		if (rightScalar == 0)
			return error_t("Division by zero");

		return leftMatrix / rightScalar;
	}

	Matrix rightMatrix = ResultToMatrix(rightResult);

	if(!rightMatrix.IsSquare())
		return error_t("Cannot perform division because divisor matrix is not square");

	if (rightMatrix.IsSingular())
		return error_t("Cannot perform division because divisor matrix is singular");
	

	if (IsMatrix(leftResult) && IsMatrix (rightResult))
	{
		return GetMatrixDivision(ResultToMatrix(leftResult), ResultToMatrix(rightResult));
	}

	return  rightMatrix.Inverse() * ResultToMatrix(leftResult);
	
}

Result DivisionNode::GetMatrixDivision(const Matrix& left, const Matrix& right) const
{
	if (!left.IsSquare())
		return error_t("Cannot perform division because dividend matrix is singular");

	if (left.Rows != right.Rows)
		return error_t("Both matrices must have the same size to perform division");

	return left * right.Inverse();
}