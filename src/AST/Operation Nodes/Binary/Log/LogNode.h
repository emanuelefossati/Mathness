#pragma once 

#include "./AST/Node.h"

class LogNode : public IBinaryNode
{
	LogNode() = default;

	Result GetResult() const override;
};