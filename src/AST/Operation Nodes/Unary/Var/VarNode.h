#pragma once

#include "./AST/Node.h"

class VarNode : public IUnaryNode
{
public:
	VarNode() = default;
	EvaluationResult GetResult() const override;
};