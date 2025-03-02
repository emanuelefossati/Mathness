#pragma once

#include "../../../Node.h"

class SumNode : public IBinaryNode
{
public:
	SumNode() = default;

	EvaluationResult GetResult() const override;

	virtual const int GetPriority() const override { return 1;}
	

private:
	static bool CanMatricesBeSummed(const Matrix& left, const Matrix& right);
};
