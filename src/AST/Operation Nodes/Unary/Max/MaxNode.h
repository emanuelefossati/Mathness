#pragma once

#include "./AST/Node.h"

class MaxNode : public IUnaryNode
{
public:
	MaxNode() = default;
	EvaluationResult GetResult() const override;
};