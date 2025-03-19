#pragma once

#include "../../Node.h"

using ListElementNode = std::tuple<std::shared_ptr<INode>, std::optional<std::shared_ptr<INode>>>;

class ListNode : public INode
{
public:
	ListNode() = default;
	

	void SetElements(std::vector<ListElementNode> elements) { _Elements = elements; }
	
	EvaluationResult GetResult() const override;
private:
		
	//std::vector<std::shared_ptr<INode>> _Elements;
	std::vector<ListElementNode> _Elements;
};