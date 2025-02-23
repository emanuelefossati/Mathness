#pragma once

#include "./AST/Node.h"

class DotNode : public IBinaryNode
{
public:
	DotNode() = default;

	EvaluationResult GetResult() const override;
};

