#pragma once
#ifndef READNAMES_H
#define SHARED_LIB_H
#include <string>
#include <functional>

/// <summary>
/// Use this delegate type to define a callback function for processing names read from a file.
/// </summary>
/// <delegate>FNameRead</delegate>
/// <description>A function pointer type for a callback that processes names read from a file.</description>
/// <param name="firstName">The first name read from the file.</param>
/// <param name="lastName">The last name read from the file.</param>
/// <returns>Returns true to continue reading names, or false to stop.</returns>
typedef bool (*FNameRead)(const std::string& firstName, const std::string& lastName);

/// <summary>
/// Use this function to read names from a specified file and process them using a callback function.
/// </summary>
///	<function>readNamesFromFile</function>
///	<description>Reads names from a specified file and invokes a callback for each name read.</description>
/// <param name="aFilename">The path to the file containing names.</param>
/// <param name="aOnNameRead">A callback function that is called for each name read. It takes two parameters: firstName and lastName. If the callback returns false, the reading process stops.</param>
/// <param name="firstName">The first name read from the file.</param>
/// <param name="lastName">The last name read from the file.</param>
/// <returns>None.</returns>			
void readNamesFromFile(const std::string& aFilename, FNameRead aOnNameRead);


#endif // READNAMES_H