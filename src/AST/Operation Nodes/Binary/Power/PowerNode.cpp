#include "PowerNode.h"

Result PowerNode::GetResult() const
{
	Result leftResult = _Left->GetResult();
	Result rightResult = _Right->GetResult();

	if (IsError(leftResult))
		return leftResult;

	if (IsError(rightResult))
		return rightResult;

	if (IsList(leftResult))
		return error_t("Cannot perform power with lists");

	if (!IsScalar(rightResult))
		return error_t("Exponent must be a scalar");
	
	if (IsScalar(leftResult))
		return std::pow(ResultToScalar(leftResult), ResultToScalar(rightResult));

	//Base is a matrix

	Matrix base = ResultToMatrix(leftResult);
	scalar_t exponent = ResultToScalar(rightResult);

	if(!base.IsSquare())
		return error_t("Matrix must be square to perform power");

	if (exponent < 0 || !IsScalarInteger(exponent))
		return error_t("Exponent must be a non-negative integer");

	if (exponent == 0)
		return Matrix::Identity(base.Rows);

	return base ^ exponent;
}