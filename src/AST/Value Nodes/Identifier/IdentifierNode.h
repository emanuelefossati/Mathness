#pragma once

#include "../../Node.h"
#include "Storage/StorageHandler.h"

class IdentifierNode : public INode
{
public:
	IdentifierNode(std::string name) : _Name(name) {}

	void AddIndexExpression(std::shared_ptr<INode> indexExpression) { _IndexExpressions.push_back(indexExpression); }
	std::tuple<std::vector<int>, Error> GetIndexExpressions();

	void SetName(std::string name) { _Name = name; }
	std::string GetName() const { return _Name; }

	EvaluationResult GetResult() const;



private:
	std::vector<std::shared_ptr<INode>> _IndexExpressions;
	std::string _Name;
};