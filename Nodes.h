#pragma once

#include <memory>
#include <vector>
#include <variant>
#include <string>
#include <format>

#include "Utils.h"



struct Scalar
{
	double Value;
};

struct Matrix
{
	std::vector<double> Elements;
	
	size_t Rows;
	size_t Columns;
};

struct List 
{
	std::vector<double> Elements;
};

using Value = std::variant<Scalar, Matrix, List>;
using Result = std::variant<Value, error_t>;

class INode
{
public:
	virtual Result GetResult() const = 0;
	std::shared_ptr<INode> GetParent() const { return _Parent; }
	void SetParent(std::shared_ptr<INode> parent) { _Parent = parent; }

private:
	std::shared_ptr<INode> _Parent = nullptr;
};


class IBinaryNode : public INode
{

public:
	std::shared_ptr<INode> GetLeft() const { return _Left; }
	std::shared_ptr<INode> GetRight() const { return _Right; }

	void SetLeft(std::shared_ptr<INode> left) { _Left = left; }
	void SetRight(std::shared_ptr<INode> right) { _Right = right; }

private:
	std::shared_ptr<INode> _Left = nullptr;
	std::shared_ptr<INode> _Right = nullptr;
};


class IUnaryNode : public INode
{
public:
	void SetChild(std::shared_ptr<INode> child) { _Child = child; }
	std::shared_ptr<INode> GetChild() const { return _Child; }

private:
	std::shared_ptr<INode> _Child = nullptr;
};


class ScalarNode : public INode
{
public:
	ScalarNode(double value) : _Value(value) {}

	Result GetResult() const override
	{
		return Scalar{ _Value };
	}
private:
	double _Value;

};

class MatrixNode : public INode
{

public:
	MatrixNode(std::vector<std::shared_ptr<INode>> elements, size_t rows, size_t columns) : _Elements(elements), _Rows(rows), _Columns(columns) {}

	Result GetResult() const override
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

			if (std::holds_alternative<Scalar>(result))
				matrix.Elements.push_back(std::get<Scalar>(result).Value);

			else
				return error_t(std::format("cannot resolve element inside matrix at ({};{})"), currentRow, currentColumn);
		}

		return matrix;
	}

private:
	std::vector<std::shared_ptr<INode>> _Elements;
	
	static size_t _Count;

	int _Id;

	size_t _Id;
	size_t _Rows;
	size_t _Columns;
};

class MatrixNode : public INode
{

public:
	MatrixNode(std::vector<std::shared_ptr<INode>> elements, size_t rows, size_t columns) : _Elements(elements), _Rows(rows), _Columns(columns) {}



	Result GetResult() const override
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

			if (std::holds_alternative<Scalar>(result))
				matrix.Elements.push_back(std::get<Scalar>(result).Value);

			else
				return error_t(std::format("cannot resolve element inside matrix at ({};{})"), currentRow, currentColumn);
		}

		return matrix;
	}

private:
	std::vector<std::shared_ptr<INode>> _Elements;
	
	static size_t _Count;

	id_t _Id;

	size_t _Id;
	size_t _Rows;
	size_t _Columns;
};

class ListNode : public INode
{
public:
	ListNode() 
	{ 
		ListNode::_Count++;
		_Id = _Count;
	}

	std::vector<std::shared_ptr<INode>> GetElements() const { return _Elements; }
	void SetElements(std::vector<std::shared_ptr<INode>> elements) { _Elements = elements; }

	Result GetResult() const override
	{
		List list;

		for (int i = 0; i < _Elements.size(); i++)
		{
			auto& element = _Elements[i];
			auto result = element->GetResult();

			if (std::holds_alternative<error_t>(result))
				return result;

			if (std::holds_alternative<Scalar>(result))
				list.Elements.push_back(std::get<Scalar>(result).Value);

			else
				return error_t(std::format("cannot resolve element inside list #{}, at index {}"), _Id, i);
		}

		return list;
	}
private:
	
	std::vector<std::shared_ptr<INode>> _Elements;
	
	static size_t _Count;
	id_t _Id;
};