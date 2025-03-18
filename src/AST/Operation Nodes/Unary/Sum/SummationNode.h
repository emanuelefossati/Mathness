#pragma once

#include "./AST/Node.h"

class SummationNode : public IUnaryNode
{
public:
	SummationNode() = default;
	EvaluationResult GetResult() const override;
};