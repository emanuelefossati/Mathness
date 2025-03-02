#pragma once

#include <memory>
#include <vector>
#include <variant>
#include <string>
#include <format>
#include <assert.h>

#include "./Results/Result.h"

class INode
{
public:
	virtual EvaluationResult GetResult() const = 0;
	std::shared_ptr<INode> GetParent() const { return _Parent; }
	void SetParent(std::shared_ptr<INode> parent) { _Parent = parent; }

	Range GetRange() const { return _TokenRange; }
	void SetRange(Range range) { _TokenRange = range; }

	virtual const int GetPriority() const 
	{
		return -1;
	}


protected:
	std::shared_ptr<INode> _Parent = nullptr;
	Range _TokenRange;
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
};

class IUnaryNode : public INode
{
public:
	void SetChild(std::shared_ptr<INode> child) { _Child = child; }
	std::shared_ptr<INode> GetChild() const { return _Child; }

protected:
	std::shared_ptr<INode> _Child = nullptr;
};