#include <iostream>
#include "./App/App.h"
#include <fmt/core.h>

int main()
{
	fmt::println("Mathness v1.0 (Type 'exit' to stop the program)");

	App::GetInstance().Run();

	return 0;
}