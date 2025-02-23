#pragma once

#include "./AST/Node.h"

class ModulusNode : public IBinaryNode
{
public:

	ModulusNode() = default;

	EvaluationResult GetResult() const override;
};