#pragma once

#include "./AST/Node.h"

class CrossNode : public IBinaryNode
{
public:
	CrossNode() = default;

	EvaluationResult GetResult() const override;
};