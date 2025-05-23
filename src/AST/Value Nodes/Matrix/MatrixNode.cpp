#include "MatrixNode.h"

MatrixNode::MatrixNode()
{
	_Rows = 0;
	_Columns = 0;
}

EvaluationResult MatrixNode::GetResult() const
{
	Matrix matrix;
	matrix.Rows = _Rows;
	matrix.Columns = _Columns;

	for (int i = 0; i < _Elements.size(); i++)
	{
		auto& element = _Elements[i];

		size_t currentRow = i / _Columns;
		size_t currentColumn = i % _Columns;

		auto result = element->GetResult();

		if (result.IsError())
			return result;

		if (result.IsScalar())
		{
			auto scalar = result.ToScalar();
			matrix.Elements.push_back(scalar);
		}

		else
			return Error("cannot resolve element inside matrix");
	}

	if(matrix.Elements.size() == 1)
		return matrix(0, 0);

	return matrix;
}