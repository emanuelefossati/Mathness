#pragma once

#include "./AST/Node.h"

class FactorialNode : public IUnaryNode
{
public:
	FactorialNode() = default;

	Result GetResult() const override;

private:
	scalar_t Factorial(scalar_t n) const;
};