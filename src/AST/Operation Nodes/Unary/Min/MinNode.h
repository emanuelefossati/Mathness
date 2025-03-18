#pragma once

#include "./AST/Node.h"

class MinNode : public IUnaryNode
{
public:
	MinNode() = default;
	EvaluationResult GetResult() const override;
};