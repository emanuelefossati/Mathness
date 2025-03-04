#include "App.h"
#include <iostream>
#include <string>

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

		//std::cout << "Tokens: " << std::endl;
		//for (auto& token : tokens)
		//{
		//	std::cout << token.ToString() << std::endl;
		//}

		Parser::GetInstance().Parse(tokens);
	}
}
