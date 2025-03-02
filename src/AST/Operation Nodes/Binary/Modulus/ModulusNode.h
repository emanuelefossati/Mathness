#pragma once

#include "./AST/Node.h"

class ModulusNode : public IBinaryNode
{
public:

	ModulusNode() = default;

	EvaluationResult GetResult() const override;

	virtual const int GetPriority() const override { return 2; }

};