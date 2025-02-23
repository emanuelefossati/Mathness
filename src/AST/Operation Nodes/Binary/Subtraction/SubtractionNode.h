#pragma once

#include "../../../Node.h"

class SubtractionNode : public IBinaryNode
{
public:
	SubtractionNode() = default;

	EvaluationResult GetResult() const override;
	

private:
	static bool CanMatricesBeSubtracted(const Matrix& left, const Matrix& right);
};
