#include "DivisionNode.h"

Result DivisionNode::GetResult() const
{
	auto leftResult = _Left->GetResult();
	auto rightResult = _Right->GetResult();

	if (leftResult.IsError())
		return leftResult;

	if (rightResult.IsError())	
		return rightResult;

	if (leftResult.IsList() || rightResult.IsList())
		return error_t("Cannot perform division with lists");

	if (leftResult.IsScalar() && rightResult.IsScalar())
	{
		scalar_t leftScalar = leftResult.ToScalar();
		scalar_t rightScalar = rightResult.ToScalar();

		if (rightScalar == 0)
			return error_t("Division by zero");

		return leftScalar / rightScalar;
	}

	if (!rightResult.IsMatrix())
	{
		scalar_t rightScalar = rightResult.ToScalar();
		Matrix leftMatrix = leftResult.ToMatrix();

		if (rightScalar == 0)
			return error_t("Division by zero");

		return leftMatrix / rightScalar;
	}

	Matrix rightMatrix = rightResult.ToMatrix();

	if(!rightMatrix.IsSquare())
		return error_t("Cannot perform division because divisor matrix is not square");

	if (rightMatrix.IsSingular())
		return error_t("Cannot perform division because divisor matrix is singular");
	

	if (leftResult.IsMatrix() && rightResult.IsMatrix())
	{
		return GetMatrixDivision(leftResult.ToMatrix(), rightResult.ToMatrix());
	}

	return  rightMatrix.Inverse() * leftResult.ToScalar();
	
}

Result DivisionNode::GetMatrixDivision(const Matrix& left, const Matrix& right) const
{
	if (!left.IsSquare())
		return error_t("Cannot perform division because dividend matrix is singular");

	if (left.Rows != right.Rows)
		return error_t("Both matrices must have the same size to perform division");

	return left * right.Inverse();
}