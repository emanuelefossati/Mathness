#pragma once

#include "./AST/Node.h"

class CrossNode : public IBinaryNode
{
public:
	CrossNode() = default;

	Result GetResult() const override;
};