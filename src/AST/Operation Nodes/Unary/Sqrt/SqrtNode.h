#pragma once

#include "./AST/Node.h"

class SqrtNode : public IUnaryNode
{
public:
	SqrtNode() = default;

	Result GetResult() const override;
};