#pragma once

#include "./AST/Node.h"

class ModulusNode : public IBinaryNode
{
public:

	ModulusNode() = default;

	Result GetResult() const override;
};