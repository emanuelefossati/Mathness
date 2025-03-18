#pragma once

#include "./AST/Node.h"

class DistinctNode : public IUnaryNode
{
public:
	DistinctNode() = default;
	EvaluationResult GetResult() const override;
};