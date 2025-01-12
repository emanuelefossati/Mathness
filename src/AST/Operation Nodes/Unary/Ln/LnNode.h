#pragma once

#include "./AST/Node.h"

class LnNode : public IUnaryNode
{
public:
	LnNode() = default;

	Result GetResult() const override;
};