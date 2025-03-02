#pragma once
#include "./Lexer/Lexer.h"
#include "./Parser/Parser.h"

class App
{
public:
	void Run();
	static App& GetInstance();

private:
	App() = default;
};

