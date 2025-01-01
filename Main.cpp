#include <iostream>
#include "App.h"

int main()
{
	std::cout << "Mathness v.0.1" << std::endl;

	App::GetInstance().Run();

	return 0;
}
