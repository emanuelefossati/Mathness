#pragma once

#include <memory>
#include <vector>
#include <variant>
#include <string>
#include <format>

#include "../Utils/TypeDefinitions.h"


struct Matrix
{
	std::vector<scalar_t> Elements;
	
	size_t Rows = 0;
	size_t Columns = 0;
};

struct List 
{
	std::vector<scalar_t> Elements;
};


using Result = std::variant<scalar_t, Matrix, List, error_t>;

class INode
{
public:
	virtual Result GetResult() const = 0;
	std::shared_ptr<INode> GetParent() const { return _Parent; }
	void SetParent(std::shared_ptr<INode> parent) { _Parent = parent; }

protected:
	std::shared_ptr<INode> _Parent = nullptr;

	static bool IsScalar(const Result& result)
	{
		return std::holds_alternative<scalar_t>(result);
	}

	static bool IsMatrix(const Result& result)
	{
		return std::holds_alternative<Matrix>(result);
	}

	static bool IsList(const Result& result)
	{
		return std::holds_alternative<List>(result);
	}
};


class IBinaryNode : public INode
{

public:
	std::shared_ptr<INode> GetLeft() const { return _Left; }
	std::shared_ptr<INode> GetRight() const { return _Right; }

	void SetLeft(std::shared_ptr<INode> left) { _Left = left; }
	void SetRight(std::shared_ptr<INode> right) { _Right = right; }




protected:
	std::shared_ptr<INode> _Left = nullptr;
	std::shared_ptr<INode> _Right = nullptr;

	static bool AreBothScalars(const Result& left, const Result& right)
	{
		return std::holds_alternative<scalar_t>(left) && std::holds_alternative<scalar_t>(right);
	}

	static bool AreBothMatrices(const Result& left, const Result& right)
	{
		return std::holds_alternative<Matrix>(left) && std::holds_alternative<Matrix>(right);
	}

	static bool AreBothLists(const Result& left, const Result& right)
	{
		return std::holds_alternative<List>(left) && std::holds_alternative<List>(right);
	}
};


class IUnaryNode : public INode
{
public:
	void SetChild(std::shared_ptr<INode> child) { _Child = child; }
	std::shared_ptr<INode> GetChild() const { return _Child; }

protected:
	std::shared_ptr<INode> _Child = nullptr;


};