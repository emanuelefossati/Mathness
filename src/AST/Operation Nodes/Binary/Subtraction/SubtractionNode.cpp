#include "SubtractionNode.h"


EvaluationResult SubtractionNode::GetResult() const
{
	EvaluationResult leftResult = _Left->GetResult();

	if (leftResult.IsError())
		return leftResult;

	EvaluationResult rightResult = _Right->GetResult();

	if (rightResult.IsError())
		return rightResult;

	if (leftResult.IsList() || rightResult.IsList())
		return Error("Cannot perform subtraction with lists", _TokenRange);

	if (leftResult.IsScalar() && rightResult.IsScalar())
	{
		scalar_t leftScalar = leftResult.ToScalar();
		scalar_t rightScalar = rightResult.ToScalar();

		return leftScalar - rightScalar;
	}

	if (leftResult.IsMatrix() && rightResult.IsMatrix())
	{
		Matrix leftMatrix = leftResult.ToMatrix();
		Matrix rightMatrix = rightResult.ToMatrix();

		if (!CanMatricesBeSubtracted(leftMatrix, rightMatrix))
			return Error("Cannot subtract matrices with different dimensions", _TokenRange);

		return leftMatrix - rightMatrix;
	}
	
	return Error("Cannot subtract a scalar with a matrix", _TokenRange);
}

bool SubtractionNode::CanMatricesBeSubtracted(const Matrix& left, const Matrix& right)
{
	return left.Rows == right.Rows && left.Columns == right.Columns;
}