#pragma once

#include "../../Node.h"
#include "Storage/StorageHandler.h"

class IdentifierNode : public INode
{
public:
	IdentifierNode(std::string name) : _Name(name) {}

	void AddIndexExpression(std::shared_ptr<INode> indexExpression) { _IndexExpressions.push_back(indexExpression); }
	std::vector<std::shared_ptr<INode>>& GetIndexExpressions() { return _IndexExpressions; }

	void SetName(std::string name) { _Name = name; }
	std::string GetName() const { return _Name; }

	EvaluationResult GetResult() const override { return _Name; }



private:
	std::vector<std::shared_ptr<INode>> _IndexExpressions;
	std::string _Name;
};