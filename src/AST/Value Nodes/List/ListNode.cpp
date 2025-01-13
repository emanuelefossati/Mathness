#include "ListNode.h"

size_t ListNode::_Count = 0;

ListNode::ListNode()
{
	_Id = _Count;
	_Count++;
}

Result ListNode::GetResult() const
{
	List list;

	for (int i = 0; i < _Elements.size(); i++)
	{
		auto& element = _Elements[i];
		auto result = element->GetResult();

		if (result.IsError())
			return result;


		if (result.IsScalar())
		{
			scalar_t scalar = result.ToScalar();
			list.Elements.emplace_back(scalar);
		}

		else
			return error_t(std::format("cannot resolve element inside list #{}, at index {}", _Id, i));
	}

	return list;
}