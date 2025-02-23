#pragma once

#include "./AST/Node.h"

class ArctanNode : public IUnaryNode
{
public:
	ArctanNode() = default;
	EvaluationResult GetResult() const override;
};