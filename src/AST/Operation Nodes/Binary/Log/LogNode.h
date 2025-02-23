#pragma once 

#include "./AST/Node.h"

class LogNode : public IBinaryNode
{
public:
	LogNode() = default;

	EvaluationResult GetResult() const override;
};