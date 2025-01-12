#pragma once

#include "./AST/Node.h"

class CosNode : public IUnaryNode
{
public:
	CosNode() = default;

	Result GetResult() const override;
};