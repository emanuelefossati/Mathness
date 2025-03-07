#pragma once
#include "./Lexer/Lexer.h"
#include "./Parser/Parser.h"

#include <fmt/core.h>
#include <fmt/color.h>

class App
{
public:
	void Run();
	static App& GetInstance();

private:
	App() = default;
};

