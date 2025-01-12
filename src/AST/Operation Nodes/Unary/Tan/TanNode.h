#pragma once

#include "./AST/Node.h"

class TanNode : public IUnaryNode
{
public:
	TanNode() = default;

	Result GetResult() const override;
};