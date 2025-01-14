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
		return Error("Cannot perform sum with lists", _TokenRange);

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
			return Error("Cannot add matrices with different dimensions", _TokenRange);

		return (_Operation == AlgebraicSumOperation::ADD) ? leftMatrix + rightMatrix : leftMatrix - rightMatrix;
	}
	
	return Error("Cannot sum a scalar with a matrix", _TokenRange);
}

bool AlgebraicSumNode::CanMatricesBeSummed(const Matrix& left, const Matrix& right)
{
	return left.Rows == right.Rows && left.Columns == right.Columns;
}