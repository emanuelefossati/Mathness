#pragma once

#include "../../../Node.h"

class SumNode : public IBinaryNode
{
public:
	SumNode() = default;

	EvaluationResult GetResult() const override;
	

private:
	static bool CanMatricesBeSummed(const Matrix& left, const Matrix& right);
};
