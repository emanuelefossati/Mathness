#include "SumNode.h"


Result AlgebraicSumNode::GetResult() const
{
	Result leftResult = _Left->GetResult();

	if (leftResult.IsError())
		return leftResult;

	Result rightResult = _Right->GetResult();

	if (rightResult.IsError())
		return rightResult;

	if (leftResult.IsList() || rightResult.IsList())
		return error_t("Cannot perform sum with lists");

	if (leftResult.IsScalar() && rightResult.IsScalar())
	{
		scalar_t leftScalar = leftResult.ToScalar();
		scalar_t rightScalar = rightResult.ToScalar();

		return (_Operation == AlgebraicSumOperation::ADD) ? leftScalar + rightScalar : leftScalar - rightScalar;
	}

	if (leftResult.IsMatrix() && rightResult.IsMatrix())
	{
		Matrix leftMatrix = leftResult.ToMatrix();
		Matrix rightMatrix = rightResult.ToMatrix();

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