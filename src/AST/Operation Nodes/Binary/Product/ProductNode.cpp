#include "ProductNode.h"

Result ProductNode::GetResult() const
{
	Result leftResult = _Left->GetResult();
	Result rightResult = _Right->GetResult();

	if (IsError(leftResult))
		return leftResult;


	if (IsError(rightResult))
		return rightResult;

	if (IsList(leftResult) || IsList(rightResult))
		return error_t("Cannot perform product with lists");

	if (IsScalar(leftResult) && IsScalar(rightResult))
	{
		scalar_t leftScalar = ResultToScalar(leftResult);
		scalar_t rightScalar = ResultToScalar(rightResult);

		return leftScalar * rightScalar;
	}

	if (IsMatrix(leftResult) && IsMatrix(rightResult))
	{
		Matrix leftMatrix = ResultToMatrix(leftResult);
		Matrix rightMatrix = ResultToMatrix(rightResult);

		if (!CanMatricesBeMultiplied(leftMatrix, rightMatrix))
			return error_t("Cannot multiply matrices with incompatible dimensions");

		Matrix result = leftMatrix * rightMatrix;

		// If the result is a 1x1 matrix, return the scalar value
		if (result.Columns == 1 && result.Rows == 1)
			return result(0, 0);

		return result;
	}

	// Scalar * Matrix
	auto [scalar, matrix] = RetrieveScalarAndMatrix(leftResult, rightResult);

	return matrix * scalar;
}

bool ProductNode::CanMatricesBeMultiplied(const Matrix& left, const Matrix& right)
{
	return left.Columns == right.Rows;
}
