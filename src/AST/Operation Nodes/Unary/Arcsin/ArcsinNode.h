#pragma once

#include "./AST/Node.h"

class ArcsinNode : public IUnaryNode
{
public:
	ArcsinNode() = default;
	Result GetResult() const override;
};