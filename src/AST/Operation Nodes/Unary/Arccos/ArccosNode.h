#pragma once

#include "./AST/Node.h"

class ArccosNode : public IUnaryNode
{
public:
	ArccosNode() = default;
	EvaluationResult GetResult() const override;
};