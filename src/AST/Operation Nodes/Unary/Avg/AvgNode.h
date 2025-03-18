#pragma once

#include "./AST/Node.h"

class AvgNode : public IUnaryNode
{
public:
	AvgNode() = default;
	EvaluationResult GetResult() const override;
};