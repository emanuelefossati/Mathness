#include "App.h"
#include <iostream>
#include <string>

App& App::GetInstance()
{
	static App instance;
	return instance;
}

void App::Run()
{
	std::string input;
	
	while (true)
	{
		std::cout << ">> ";
		std::getline(std::cin, input);

		if (input == "exit")
		{
			break;
		}

		printf("%s\n", input.c_str());

		auto [tokens, error] = Lexer::GetInstance().Lex(input);

		if (!error.empty())
		{
			std::cout << error << std::endl;
			continue;
		}

		for (auto& token : tokens)
		{
			std::cout << token.ToString() << std::endl;
		}

		//parsing

		//evaluation

		//execution
		
	}
}
