#pragma once

#include "./AST/Node.h"

class SinNode : public IUnaryNode
{
public:
	SinNode() = default;

	EvaluationResult GetResult() const override;
};