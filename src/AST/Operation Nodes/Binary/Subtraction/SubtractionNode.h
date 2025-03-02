#pragma once

#include "../../../Node.h"

class SubtractionNode : public IBinaryNode
{
public:
	SubtractionNode() = default;

	EvaluationResult GetResult() const override;

	virtual const int GetPriority() const override { return 1; }

private:
	static bool CanMatricesBeSubtracted(const Matrix& left, const Matrix& right);
};
