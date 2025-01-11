#pragma once

#include "./AST/Node.h"

class PowerNode : public IBinaryNode 
{
public:
	PowerNode() = default;

	Result GetResult() const override;
};