#include <iostream>
#include "App.h"

int main()
{
	std::cout << "Mathness v0.1" << std::endl;

	App::GetInstance().Run();

	return 0;
}
