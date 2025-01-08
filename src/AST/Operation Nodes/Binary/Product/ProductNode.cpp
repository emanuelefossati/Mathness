#include "ProductNode.h"

Result ProductNode::GetResult() const
{
	Result leftResult = _Left->GetResult();

	if (std::holds_alternative<error_t>(leftResult))
		return leftResult;

	Result rightResult = _Right->GetResult();

	if (std::holds_alternative<error_t>(rightResult))
		return rightResult;

	if (std::holds_alternative<List>(leftResult) || std::holds_alternative<List>(rightResult))
		return error_t("Cannot perform product with lists");

	if (AreBothScalars(leftResult, rightResult))
	{
		scalar_t leftScalar = std::get<scalar_t>(leftResult);
		scalar_t rightScalar = std::get<scalar_t>(rightResult);

		return leftScalar * rightScalar;
	}

	if (AreBothMatrices(leftResult, rightResult))
	{
		Matrix leftMatrix = std::get<Matrix>(leftResult);
		Matrix rightMatrix = std::get<Matrix>(rightResult);

		if (!CanMatricesBeMultiplied(leftMatrix, rightMatrix))
			return error_t("Cannot multiply matrices with incompatible dimensions");

		Matrix result = leftMatrix * rightMatrix;

		// If the result is a 1x1 matrix, return the scalar value
		if (result.Columns == 1 && result.Rows == 1)
			return result(0, 0);

		return result;
	}

	// Scalar * Matrix
	auto [scalar, matrix] = GetParams(leftResult, rightResult);

	return matrix * scalar;
}

std::tuple<scalar_t, Matrix> ProductNode::GetParams(const Result& leftResult, const Result& rightResult) const
{
	assert(IsScalar(leftResult) != IsScalar(rightResult));
	assert(IsMatrix(leftResult) != IsMatrix(rightResult));

	if (IsScalar(leftResult))
	{
		scalar_t leftScalar = std::get<scalar_t>(leftResult);
		Matrix rightMatrix = std::get<Matrix>(rightResult);

		return { leftScalar, rightMatrix };
	}
	else
	{
		Matrix leftMatrix = std::get<Matrix>(leftResult);
		scalar_t rightScalar = std::get<scalar_t>(rightResult);

		return { rightScalar, leftMatrix };
	}
}

bool ProductNode::CanMatricesBeMultiplied(const Matrix& left, const Matrix& right)
{
	return left.Columns == right.Rows;
}
