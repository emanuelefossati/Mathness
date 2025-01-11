#include "SumNode.h"


Result AlgebraicSumNode::GetResult() const
{
	Result leftResult = _Left->GetResult();

	if (IsError(leftResult))
		return leftResult;

	Result rightResult = _Right->GetResult();

	if (IsError(rightResult))
		return rightResult;

	if (IsList(leftResult) || IsList(rightResult))
		return error_t("Cannot perform sum with lists");

	if (IsScalar(leftResult) && IsScalar(rightResult))
	{
		scalar_t leftScalar = ResultToScalar(leftResult);
		scalar_t rightScalar = ResultToScalar(rightResult);

		return (_Operation == AlgebraicSumOperation::ADD) ? leftScalar + rightScalar : leftScalar - rightScalar;
	}

	if (IsMatrix(leftResult) && IsMatrix(rightResult))
	{
		Matrix leftMatrix = ResultToMatrix(leftResult);
		Matrix rightMatrix = ResultToMatrix(rightResult);

		if (!CanMatricesBeSummed(leftMatrix, rightMatrix))
			return error_t("Cannot add matrices with different dimensions");

		return (_Operation == AlgebraicSumOperation::ADD) ? leftMatrix + rightMatrix : leftMatrix - rightMatrix;
	}
	
	return error_t("Cannot sum a scalar with a matrix");
}

bool AlgebraicSumNode::CanMatricesBeSummed(const Matrix& left, const Matrix& right)
{
	return left.Rows == right.Rows && left.Columns == right.Columns;
}