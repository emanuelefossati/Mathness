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
			return EvaluationError("Index expression must be a scalar");
		}

		indices.push_back(expressionResult.GetScalar());
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
			return EvaluationError("Scalar value cannot be indexed");
		}	
		
		return value;
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
			return matrix.Elements[indices.size[0]];
		}

		return EvaluationError("Vector cannot be indexed with more than one index");
	}

	if (indices.empty())
	{
		return matrix;
	}


	if (indices.size() != 2)
	{
		return EvaluationError("Matrix must be indexed with two indices");
	}

	size_t row = indices[0];
	size_t column = indices[1];

	if (row >= matrix.Rows)
	{
		return EvaluationError("Row index out of bounds");
	}

	if (column >= matrix.Columns)
	{
		return EvaluationError("Column index out of bounds");
	}

	return matrix(row, column);
}