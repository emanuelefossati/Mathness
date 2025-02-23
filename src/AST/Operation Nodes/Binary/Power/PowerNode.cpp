#include "PowerNode.h"

EvaluationResult PowerNode::GetResult() const
{
	EvaluationResult leftResult = _Left->GetResult();
	EvaluationResult rightResult = _Right->GetResult();

	if (leftResult.IsError())
		return leftResult;

	if (rightResult.IsError())
		return rightResult;

	if (leftResult.IsList() || rightResult.IsList())
		return Error("Cannot perform power with lists", _TokenRange);

	if (!rightResult.IsScalar())
		return Error("Exponent must be a scalar", _TokenRange);

	scalar_t exponent = rightResult.ToScalar();
	
	if (leftResult.IsScalar())
		return std::pow(leftResult.ToScalar(), exponent);

	//Base is a matrix

	Matrix base = leftResult.ToMatrix();
	
	if(!base.IsSquare())
		return Error("Matrix must be square to perform power", _TokenRange);

	if (!EvaluationResult::IsScalarInteger(exponent))
		return Error("Exponent must be a non-negative integer", _TokenRange);

	if (exponent == 0)
		return Matrix::Identity(base.Rows);

	return base ^ exponent;
}