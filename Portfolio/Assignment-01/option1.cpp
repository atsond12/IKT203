// Option 1 (Standard): Console Text Editor.
//
#include <iostream>
#include <string>
#include "option1.h"

//typedef [return] [*funksjons_name] [argument_liste]
typedef void (*FOnCharRead) (char, int, int);

static void parseText(std::string& aText, FOnCharRead aOnCharRead) {
	int length = static_cast<int> (aText.size());
	for (int i = 0; i < length; i++) {
		aOnCharRead(aText[i], i, length);
	}
}

static void charOnRead(char aChar, int aIndex, int aLength) {
	std::cout << "Parsed char " << aChar << " at index " << aIndex << " of " << aLength << std::endl;
}

int RunApp() {
	std::string text = "Hello, world!";
	parseText(text, charOnRead);
	return 0;
}