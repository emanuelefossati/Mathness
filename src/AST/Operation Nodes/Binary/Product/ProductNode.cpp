#include "ProductNode.h"

Result ProductNode::GetResult() const
{
	Result leftResult = _Left->GetResult();
	Result rightResult = _Right->GetResult();

	if (leftResult.IsError())
		return leftResult;


	if (rightResult.IsError())
		return rightResult;

	if (leftResult.IsList() || rightResult.IsList())
		return error_t("Cannot perform product with lists");

	if (leftResult.IsScalar() && rightResult.IsScalar())
	{
		scalar_t leftScalar = leftResult.ToScalar();
		scalar_t rightScalar = rightResult.ToScalar();

		return leftScalar * rightScalar;
	}

	if (leftResult.IsMatrix() && rightResult.IsMatrix())
	{
		Matrix leftMatrix = leftResult.ToMatrix();
		Matrix rightMatrix = rightResult.ToMatrix();

		if (!CanMatricesBeMultiplied(leftMatrix, rightMatrix))
			return error_t("Cannot multiply matrices with incompatible dimensions");

		Matrix result = leftMatrix * rightMatrix;

		// If the result is a 1x1 matrix, return the scalar value
		if (result.Columns == 1 && result.Rows == 1)
			return result(0, 0);

		return result;
	}

	// Scalar * Matrix
	auto [scalar, matrix] = Result::RetrieveScalarAndMatrix(leftResult, rightResult);

	return matrix * scalar;
}

bool ProductNode::CanMatricesBeMultiplied(const Matrix& left, const Matrix& right)
{
	return left.Columns == right.Rows;
}
