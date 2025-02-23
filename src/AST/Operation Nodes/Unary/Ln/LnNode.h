#pragma once

#include "./AST/Node.h"

class LnNode : public IUnaryNode
{
public:
	LnNode() = default;

	EvaluationResult GetResult() const override;
};