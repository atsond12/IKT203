#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "ReadNames.h"

void readNamesFromFile(const std::string& aFilename, FNameRead aOnNameRead)
{
	if (aFilename.empty()) return;
	std::ifstream file(aFilename);
	if (!file.is_open())
	{
		std::cerr << "Error opening file: " << aFilename << std::endl;
		return;
	}
	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string firstName, lastName;
		if (iss >> firstName >> lastName)
		{
			if (aOnNameRead) // If the callback is set, call it
			{
				//If the function returns false, stop reading further
				if (!aOnNameRead(firstName, lastName)) break;
			}
		}
	}
	file.close();
}
