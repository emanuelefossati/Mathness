#pragma once

#include "./AST/Node.h"

class ProductNode : public IBinaryNode
{
public:
	ProductNode() = default;
	

	EvaluationResult GetResult() const override;

private:
	static bool CanMatricesBeMultiplied(const Matrix& left, const Matrix& right);
};