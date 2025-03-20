#include "IdentifierNode.h"

EvaluationResult IdentifierNode::GetResult() const
{
	std::vector<int> indices;

	for (auto& index : _IndexExpressions)
	{
		EvaluationResult expressionResult = index->GetResult();

		if (expressionResult.IsError())
		{
			return expressionResult;
		}

		if (!expressionResult.IsScalar())
		{
			return Error("Index expression must be a scalar");
		}

		indices.push_back(expressionResult.ToScalar());
	}

	EvaluationResult value = StorageHandler::GetInstance().GetValue(_Name);

	if (value.IsError())
	{
		return value;
	}

	if (value.IsScalar())
	{
		if (!indices.empty())
		{
			return Error("Scalar value cannot be indexed");
		}	
		
		return value;
	}

	if (value.IsList())
	{
		List list = value.ToList();

		if (indices.size() > 1)
		{
			return Error("List value can not be indexed with more than one index");
		}

		if (indices.empty())
		{
			return list;
		}	


		int index = indices[0];

		if (index < 0 || index >= list.Elements.size())
		{
			return Error("Index out of bounds");
		}

		return list.Elements[index].Value;
	}

	assert(value.IsMatrix());

	Matrix matrix = value.ToMatrix();

	if (matrix.IsVector())
	{
		if (indices.empty())
		{
			return matrix;
		}

		if (indices.size() == 1)
		{
			return matrix.Elements[indices[0]];
		}

		return Error("Vector cannot be indexed with more than one index");
	}

	if (indices.empty())
	{
		return matrix;
	}


	if (indices.size() != 2)
	{
		return Error("Matrix must be indexed with two indices");
	}

	size_t row = indices[0];
	size_t column = indices[1];

	if (row >= matrix.Rows)
	{
		return Error("Row index out of bounds");
	}

	if (column >= matrix.Columns)
	{
		return Error("Column index out of bounds");
	}

	return matrix(row, column);
}

std::tuple<std::vector<int>, Error> IdentifierNode::GetIndexExpressions()
{
	std::vector<int> indices;

	for (auto& index : _IndexExpressions)
	{
		EvaluationResult expressionResult = index->GetResult();

		if (expressionResult.IsError())
		{
			return { {}, expressionResult.ToError() };
		}

		if (!expressionResult.IsScalar())
		{
			return { {}, Error("Index expression must be a scalar") };
		}

		indices.push_back(expressionResult.ToScalar());
	}

	return { indices, Error() };
}