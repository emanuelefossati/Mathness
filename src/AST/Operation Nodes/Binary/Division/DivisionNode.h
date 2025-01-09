#pragma once

#include "./AST/Node.h"

class DivisionNode : public IBinaryNode
{
public:
	DivisionNode() = default;

	Result GetResult() const override;

private:
	Result GetMatrixDivision(const Matrix& left, const Matrix& right) const;
};