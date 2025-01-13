#include "PowerNode.h"

Result PowerNode::GetResult() const
{
	Result leftResult = _Left->GetResult();
	Result rightResult = _Right->GetResult();

	if (leftResult.IsError())
		return leftResult;

	if (rightResult.IsError())
		return rightResult;

	if (leftResult.IsList() || rightResult.IsList())
		return error_t("Cannot perform power with lists");

	if (!rightResult.IsScalar())
		return error_t("Exponent must be a scalar");

	scalar_t exponent = rightResult.ToScalar();
	
	if (leftResult.IsScalar())
		return std::pow(leftResult.ToScalar(), exponent);

	//Base is a matrix

	Matrix base = leftResult.ToMatrix();
	
	if(!base.IsSquare())
		return error_t("Matrix must be square to perform power");

	if (!Result::IsScalarInteger(exponent))
		return error_t("Exponent must be a non-negative integer");

	if (exponent == 0)
		return Matrix::Identity(base.Rows);

	return base ^ exponent;
}