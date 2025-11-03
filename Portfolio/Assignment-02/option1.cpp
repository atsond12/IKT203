// Option 1 (Standard): Console Text Editor.
//

#include <iostream>
#include <string>
#include "option1.h"
#include "SharedLib.h"

/**
 * @brief Callback function to process one name.
 */
static bool NameReadCallback(const int aIndex, const int aTotalCount, const std::string& aFirstName, const std::string& aLastName)
{
	std::cout << "Reading Name " << (aIndex + 1) << " of " << aTotalCount << ": "
		<< aFirstName << " " << aLastName << "\n";

	// We only want to read 10 names (index 0 through 9)
	// Return false when aIndex is 9 to stop the loop after this one.
	return (aIndex < 9);
}


int RunApp()
{
	// Path to the names data file
	std::string filename = "F:\\IKT203\\VisualStudio\\DATA\\random_names.txt";

	std::cout << "Reading first 10 names from file: " << filename << "\n\n";

	// Call the utility function with the name callback
	readNamesFromFile(filename, NameReadCallback);

	std::cout << "\nFinished reading names." << std::endl;

	return 0;
}