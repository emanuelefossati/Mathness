#include "StorageHandler.h"

StorageHandler& StorageHandler::GetInstance()
{
	static StorageHandler instance;
	return instance;
}

std::optional<Error> StorageHandler::StoreValue(std::string_view key, EvaluationResult& value, std::vector<int> indices)
{
	if (indices.empty())
	{
		_Storage[std::string(key)] = value;

		return std::nullopt;
	}

	auto it = _Storage.find(std::string(key));

	if (it == _Storage.end())
	{

		return Error("Cannot store data inside an element of a storage that has not been previously defined");
	}

	auto& storageValue = it->second;

	if (storageValue.IsError())
	{
		return Error("Error values cannot be indexed");
	}

	if (storageValue.IsScalar())
	{
		return Error("Scalar values cannot be indexed");
	}

	if (storageValue.IsList())
	{
		if (indices.size() != 1)
		{
			return Error("List values can only be indexed with one index");
		}

		auto list = storageValue.ToList();
		int index = indices[0];

		if (index < 0 || index >= list.Elements.size())
		{
			return Error("Index out of bounds");
		}

		if (!value.IsScalar())
		{
			return Error("Only scalar values can be stored in a list");
		}
		

		list.Elements[index] = List::ListElement{value.ToScalar()};

		_Storage[std::string(key)] = list;
		
		return std::nullopt;
	}

	assert(storageValue.IsMatrix());

	auto matrix = storageValue.ToMatrix();

	if (matrix.IsVector())
	{
		if (indices.size() != 1)
		{
			return Error("Vector values can only be indexed with one index");
		}

		int index = indices[0];

		if (index < 0 || index >= matrix.Elements.size())
		{
			return Error("Index out of bounds");
		}

		if (!value.IsScalar())
		{
			return Error("Only scalar values can be stored in a vector");
		}

		matrix.Elements[index] = value.ToScalar();
		_Storage[std::string(key)] = matrix;

		return std::nullopt;
	}

	if (indices.size() != 2)
	{
		return Error("Matrix values can only be indexed with two indices");
	}

	int row = indices[0];
	int column = indices[1];

	if (row < 0 || row >= matrix.Rows)
	{
		return Error("Row index out of bounds");
	}

	if (column < 0 || column >= matrix.Columns)
	{
		return Error("Column index out of bounds");
	}

	if (!value.IsScalar())
	{
		return Error("Only scalar values can be stored in a matrix");
	}

	matrix.Elements[row * matrix.Columns + column] = value.ToScalar();
	_Storage[std::string(key)] = matrix;

	return std::nullopt;


}

EvaluationResult StorageHandler::GetValue(std::string_view key, std::vector<int> indices) const
{
	auto it = _Storage.find(std::string(key));

	if (it == _Storage.end())
	{
		return Error("Identinfier not found");
	}

	if (indices.empty())
	{
		return it->second;
	}

	auto& storageValue = it->second;

	if (storageValue.IsError())
	{
		return storageValue;
	}

	if (storageValue.IsScalar())
	{
		return Error("Scalar values cannot be indexed");
	}

	if (storageValue.IsList())
	{
		if (indices.size() != 1)
		{
			return Error("List values can only be indexed with one index");
		}

		auto list = storageValue.ToList();
		int index = indices[0];

		if (index < 0 || index >= list.Elements.size())
		{
			return Error("Index out of bounds");
		}

		return list.Elements[index].Value;
	}

	assert(storageValue.IsMatrix());

	auto matrix = storageValue.ToMatrix();

	if (matrix.IsVector())
	{
		if (indices.size() != 1)
		{
			return Error("Vector values can only be indexed with one index");
		}

		int index = indices[0];

		if (index < 0 || index >= matrix.Elements.size())
		{
			return Error("Index out of bounds");
		}

		return matrix.Elements[index];
	}

	if (indices.size() != 2)
	{
		return Error("Matrix values can only be indexed with two indices");
	}

	int row = indices[0];
	int column = indices[1];

	if (row < 0 || row >= matrix.Rows)
	{
		return Error("Row index out of bounds");
	}

	if (column < 0 || column >= matrix.Columns)
	{
		return Error("Column index out of bounds");
	}

	return matrix.Elements[row * matrix.Columns + column];

}