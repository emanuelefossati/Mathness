#pragma once

#include "./AST/Node.h"

class ArctanNode : public IUnaryNode
{
public:
	ArctanNode() = default;
	Result GetResult() const override;
};