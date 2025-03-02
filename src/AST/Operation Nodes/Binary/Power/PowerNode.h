#pragma once

#include "./AST/Node.h"

class PowerNode : public IBinaryNode 
{
public:
	PowerNode() = default;

	EvaluationResult GetResult() const override;

	virtual const int GetPriority() const override { return 3; }

};