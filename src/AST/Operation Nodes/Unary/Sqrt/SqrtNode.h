#pragma once

#include "./AST/Node.h"

class SqrtNode : public IUnaryNode
{
public:
	SqrtNode() = default;

	EvaluationResult GetResult() const override;
};