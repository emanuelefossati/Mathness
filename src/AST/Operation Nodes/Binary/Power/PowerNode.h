#pragma once

#include "./AST/Node.h"

class PowerNode : public IBinaryNode 
{
public:
	PowerNode() = default;

	EvaluationResult GetResult() const override;
};