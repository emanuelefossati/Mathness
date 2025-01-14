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
		return Error("Cannot perform division with lists", _TokenRange);

	if (leftResult.IsScalar() && rightResult.IsScalar())
	{
		scalar_t leftScalar = leftResult.ToScalar();
		scalar_t rightScalar = rightResult.ToScalar();

		if (rightScalar == 0)
			return Error("Division by zero", _TokenRange);

		return leftScalar / rightScalar;
	}

	if (!rightResult.IsMatrix())
	{
		scalar_t rightScalar = rightResult.ToScalar();
		Matrix leftMatrix = leftResult.ToMatrix();

		if (rightScalar == 0)
			return Error("Division by zero", _TokenRange);

		return leftMatrix / rightScalar;
	}

	Matrix rightMatrix = rightResult.ToMatrix();

	if(!rightMatrix.IsSquare())
		return Error("Cannot perform division because divisor matrix is not square", _TokenRange);

	if (rightMatrix.IsSingular())
		return Error("Cannot perform division because divisor matrix is singular", _TokenRange);
	

	if (leftResult.IsMatrix() && rightResult.IsMatrix())
	{
		return GetMatrixDivision(leftResult.ToMatrix(), rightResult.ToMatrix());
	}

	return  rightMatrix.Inverse() * leftResult.ToScalar();
	
}

Result DivisionNode::GetMatrixDivision(const Matrix& left, const Matrix& right) const
{
	if (!left.IsSquare())
		return Error("Cannot perform division because dividend matrix is singular", _TokenRange);

	if (left.Rows != right.Rows)
		return Error("Both matrices must have the same size to perform division", _TokenRange);

	return left * right.Inverse();
}