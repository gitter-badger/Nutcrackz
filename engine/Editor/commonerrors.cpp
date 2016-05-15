#include "commonerrors.h"

#include <iostream>

void fatalError(std::string errorString)
{

	std::cout << errorString << std::endl;
	//std::cout << "Enter any key to quit...";

	//std::cin.get();

	//exit(1);
}