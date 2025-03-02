#pragma once

#include "./AST/Node.h"

class ProductNode : public IBinaryNode
{
public:
	ProductNode() = default;
	

	EvaluationResult GetResult() const override;

	virtual const int GetPriority() const override { return 2; }


private:
	static bool CanMatricesBeMultiplied(const Matrix& left, const Matrix& right);
};