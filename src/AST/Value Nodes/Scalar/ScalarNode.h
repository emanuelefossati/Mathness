#pragma once

#include "../../Node.h"

class ScalarNode : public INode
{
public:
	ScalarNode(double value) : _Value(value) {}

	EvaluationResult GetResult() const override { return scalar_t{ _Value }; }
private:
	scalar_t _Value;

};