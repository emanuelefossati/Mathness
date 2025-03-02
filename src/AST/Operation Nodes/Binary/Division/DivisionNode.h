#pragma once

#include "./AST/Node.h"

class DivisionNode : public IBinaryNode
{
public:
	DivisionNode() = default;

	EvaluationResult GetResult() const override;

	virtual const int GetPriority() const override { return 2; }


private:
	EvaluationResult GetMatrixDivision(const Matrix& left, const Matrix& right) const;
};