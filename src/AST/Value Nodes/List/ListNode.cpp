#include "ListNode.h"

EvaluationResult ListNode::GetResult() const
{
	List list;

	for (int i = 0; i < _Elements.size(); i++)
	{
		auto& [valueNode, weightNode] = _Elements[i];

		EvaluationResult valueResult = valueNode->GetResult();
		EvaluationResult weightResult = weightNode.has_value() ? weightNode.value()->GetResult() : scalar_t{1};


		if (valueResult.IsError())
			return valueResult;

		if (!valueResult.IsScalar())
			return Error(std::format("cannot resolve element inside list"));

		if (weightResult.IsError())
			return weightResult;

		if (!weightResult.IsScalar())
			return Error(std::format("cannot resolve weight inside list"));

		list.Elements.emplace_back(List::ListElement{valueResult.ToScalar(), weightResult.ToScalar()});






			
	}

	return list;
}