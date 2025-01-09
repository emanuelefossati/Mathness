#pragma once

#include "./AST/Node.h"

class ProductNode : public IBinaryNode
{
public:
	ProductNode() = default;
	

	Result GetResult() const override;

private:
	static bool CanMatricesBeMultiplied(const Matrix& left, const Matrix& right);
	std::tuple<scalar_t, Matrix> GetParams(const Result& leftResult, const Result& rightResult) const;
};