#pragma once

#include "./AST/Node.h"

class CosNode : public IUnaryNode
{
public:
	CosNode() = default;

	EvaluationResult GetResult() const override;
};