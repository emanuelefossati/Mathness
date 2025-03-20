#include "App.h"
#include <string>
#include <iostream>
#include "Storage/StorageHandler.h"

App& App::GetInstance()
{
	static App instance;
	return instance;
}

void ClearComments(std::string& input)
{
	for (auto it = input.begin(); it != input.end(); it++)
	{
		if (*it == '#')
		{
			input.erase(it, input.end());

			return;
		}
	}
}



void App::Run()
{
	std::string input;
	
	while (true)
	{
		std::cout << ">>> ";
		std::getline(std::cin, input);

		ClearComments(input);

		if (input.empty())
		{
			continue;
		}	

		if (input == "exit")
		{
			break;
		}

		auto [tokens, error] = Lexer::GetInstance().Lex(input);

		if (error.has_value())
		{	
			App::PrintError(error.value());
			continue;
		}

		auto [leftExpressionTokenList, rightExpressionTokenList, splitError] = Parser::GetInstance().SplitTokenList(tokens);

		if (splitError.has_value())
		{
			App::PrintError(splitError.value());
			continue;
		}
		
		bool isAssignment = !rightExpressionTokenList.empty();

		if (!isAssignment)
		{
			rightExpressionTokenList = leftExpressionTokenList;
		}

		NodeResult expressionTree = Parser::GetInstance().Parse(rightExpressionTokenList);

		if (!expressionTree.IsNode())
		{
			App::PrintError(expressionTree.ToError());
			continue;
		}

		auto result = expressionTree.ToNode()->GetResult();

		if (isAssignment)
		{
			auto assignmentError = HandleAssignment(leftExpressionTokenList, result);

			if (assignmentError.has_value())
			{
				App::PrintError(assignmentError.value());
				continue;
			}

		}

		ShowResult(result);
	}
}


std::optional<Error> App::HandleAssignment(std::vector<LexingToken>& leftExpressionTokenList, EvaluationResult& rightExpressionResult)
{
	NodeResult lValue = Parser::GetInstance().ParseLValue(leftExpressionTokenList);

	if (!lValue.IsNode())
	{
		return lValue.ToError();
	}

	assert(std::dynamic_pointer_cast<IdentifierNode>(lValue.ToNode()) != nullptr);
	auto lValueNode = std::static_pointer_cast<IdentifierNode>(lValue.ToNode());


	auto [indices, indexError] = lValueNode->GetIndexExpressions();

	if (!indexError.empty())
	{
		return indexError;
	}

	auto storeError = StorageHandler::GetInstance().StoreValue(lValueNode->GetName(), rightExpressionResult, indices);

	if (storeError.has_value())
	{
		return storeError.value();
	}

	return std::nullopt;
}

void App::ShowResult(EvaluationResult& result) const
{
	if (result.IsError())
	{
		App::PrintError(result.ToError());
		return;
	}

	if (result.IsScalar())
	{
		fmt::print("{}\n", result.ToScalar());
		return;
	}

	if (result.IsMatrix())
	{
		auto matrix = result.ToMatrix();

		fmt::print("{}\n", matrix.ToString());
		return;
	}

	if (result.IsList())
	{
		auto list = result.ToList();

		fmt::print("{}\n", list.ToString());
		return;
	}
}

void App::PrintError(Error error)
{
	fmt::print(fmt::fg(fmt::color::red), "{}\n", error);
}