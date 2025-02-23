#pragma once

#include "./AST/Node.h"

class TanNode : public IUnaryNode
{
public:
	TanNode() = default;

	EvaluationResult GetResult() const override;
};