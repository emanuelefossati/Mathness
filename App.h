#pragma once
#include "Lexer.h"

class App
{
public:
	void Run();
	static App& GetInstance();

private:
	App() = default;
};

