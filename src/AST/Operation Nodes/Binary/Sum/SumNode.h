#pragma once

#include "../../../Node.h"

enum class AlgebraicSumOperation
{
	ADD,
	SUB,
};

class AlgebraicSumNode : public IBinaryNode
{
public:
	AlgebraicSumNode() = default;

	Result GetResult() const override;

	AlgebraicSumOperation GetOperation() const { return _Operation; }
	void SetOperation(AlgebraicSumOperation operation) { _Operation = operation; }
	

private:
	static bool CanMatricesBeSummed(const Matrix& left, const Matrix& right);
	Result SumMatrices(const Matrix& left, const Matrix& right) const; 

	AlgebraicSumOperation _Operation;
};


