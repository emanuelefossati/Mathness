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
		std::cout << ">> ";
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
			std::cout << error.value() << std::endl;
			continue;
		}

		auto [leftExpressionTokenList, rightExpressionTokenList, splitError] = Parser::GetInstance().SplitTokenList(tokens);

		if (splitError.has_value())
		{
			std::cout << splitError.value() << std::endl;
			continue;
		}


		NodeResult expressionTree = Parser::GetInstance().Parse(rightExpressionTokenList);

		if (!expressionTree.IsNode())
		{
			fmt::print(fmt::fg(fmt::color::red), "{}\n", expressionTree.ToError());
			continue;
		}

		auto result = expressionTree.ToNode()->GetResult();

		if (result.IsError())
		{
			fmt::print(fmt::fg(fmt::color::red), "{}\n", result.ToError());
			continue;
		}

		if (result.IsScalar())
		{
			fmt::println("Result: {}", result.ToScalar());
			continue;
		}

		if (result.IsMatrix())
		{
			auto matrix = result.ToMatrix();

			fmt::println("Result: {}", matrix.ToString());
			continue;
		}


	}
}
