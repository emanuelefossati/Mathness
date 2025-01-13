#include <iostream>
#include "./App/App.h"
#include <fmt/core.h>

int main()
{
	fmt::println("Mathness v0.1");

	App::GetInstance().Run();

	return 0;
}