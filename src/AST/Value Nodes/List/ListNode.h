#pragma once

#include "../../Node.h"

class ListNode : public INode
{
public:
	ListNode() = default;
	
	std::vector<std::shared_ptr<INode>> GetElements() const { return _Elements; }
	void SetElements(std::vector<std::shared_ptr<INode>> elements) { _Elements = elements; }
	
	Result GetResult() const override;
private:
		
	std::vector<std::shared_ptr<INode>> _Elements;
};