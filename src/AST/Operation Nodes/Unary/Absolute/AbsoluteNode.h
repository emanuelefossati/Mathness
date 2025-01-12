#pragma once

#include "./AST/Node.h"

class AbsoluteNode : public IUnaryNode
{
public:
	AbsoluteNode() = default;

	Result GetResult() const override;
};