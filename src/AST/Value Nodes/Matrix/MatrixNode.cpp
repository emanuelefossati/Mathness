#include "MatrixNode.h"

size_t MatrixNode::_Count = 0;

MatrixNode::MatrixNode()
{
	_Id = _Count;
	_Count++;

	_Rows = 0;
	_Columns = 0;
}

Result MatrixNode::GetResult() const
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

		if (std::holds_alternative<error_t>(result))
			return result;

		if (std::holds_alternative<scalar_t>(result))
		{
			auto scalar = std::get<scalar_t>(result);
			matrix.Elements.push_back(scalar);
		}

		else
			return error_t(std::format("cannot resolve element inside matrix #{} at ({};{})", _Id, currentRow, currentColumn));
	}

	return matrix;
}