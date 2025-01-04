#include "SumNode.h"




Result AlgebraicSumNode::GetResult() const
{
	Result leftResult = _Left->GetResult();

	if (std::holds_alternative<error_t>(leftResult))
		return leftResult;

	Result rightResult = _Right->GetResult();

	if (std::holds_alternative<error_t>(rightResult))
		return rightResult;

	if (std::holds_alternative<List>(leftResult) || std::holds_alternative<List>(rightResult))
		return error_t("Cannot perform sum with lists");

	if (AreBothScalars(leftResult, rightResult))
	{
		scalar_t leftScalar = std::get<scalar_t>(leftResult);
		scalar_t rightScalar = std::get<scalar_t>(rightResult);

		return (_Operation == AlgebraicSumOperation::ADD) ? leftScalar + rightScalar : leftScalar - rightScalar;
	}

	if (AreBothMatrices(leftResult, rightResult))
	{
		Matrix leftMatrix = std::get<Matrix>(leftResult);
		Matrix rightMatrix = std::get<Matrix>(rightResult);

		if (!CanMatricesBeSummed(leftMatrix, rightMatrix))
			return error_t("Cannot add matrices with different dimensions");

		return SumMatrices(leftMatrix, rightMatrix);
	}
	
	return error_t("Cannot sum a scalar with a matrix");
}

bool AlgebraicSumNode::CanMatricesBeSummed(const Matrix& left, const Matrix& right)
{
	return left.Rows == right.Rows && left.Columns == right.Columns;
}

Result AlgebraicSumNode::SumMatrices(const Matrix& left, const Matrix& right) const
{
	Matrix result;

	result.Rows = left.Rows;
	result.Columns = left.Columns;

	for (int i = 0; i < left.Elements.size(); i++)
	{
		scalar_t sum = (_Operation == AlgebraicSumOperation::ADD) ? left.Elements[i] + right.Elements[i] : left.Elements[i] - right.Elements[i];

		result.Elements.push_back(sum);
	}

	return result;
}