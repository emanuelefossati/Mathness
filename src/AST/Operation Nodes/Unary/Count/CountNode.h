#pragma once

#include "./AST/Node.h"

class CountNode : public IUnaryNode
{
public:
	CountNode() = default;

	EvaluationResult GetResult() const override;
};