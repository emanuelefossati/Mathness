#pragma once

#include "./AST/Node.h"

class ExpNode : public IUnaryNode
{
public:
	ExpNode() = default;

	EvaluationResult GetResult() const override;
};