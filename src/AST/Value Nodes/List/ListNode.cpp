#include "ListNode.h"

EvaluationResult ListNode::GetResult() const
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
			return Error(std::format("cannot resolve element inside list"), _TokenRange);
	}

	return list;
}