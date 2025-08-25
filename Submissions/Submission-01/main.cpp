// Submission-01.cpp : Defines the entry point for the application.
//

#include "main.h"
#include <iostream> 
#include "list.hpp"

using StringList = TLinkedList<std::string*>;
using namespace std;

static std::string* Factory_String1() { return new std::string("Hello"); }
static std::string* Factory_String2() { return new std::string("Data Structures"); };
static std::string* Factory_String3() { return new std::string("Linked List"); }

static void PrintNode(std::string* const* data, int index) {
	cout << "Node " << index << ": " << **data << endl;
}

int main()
{

	std::cout << "--- Testing TLinkedList with std::string* ---" << std::endl;
	
	StringList myList{};

    
	myList.Append(Factory_String1);
	myList.Append(Factory_String2);
	myList.Append(Factory_String3);

	std::cout << "List created with " << myList.GetSize() << " elements." << std::endl;
	std::cout << "Contents:" << std::endl;

	myList.ForEach(PrintNode);

	std::cout << "\n--- Cleaning up memory ---" << std::endl;
	std::cout << "Memory freed. Program finished." << std::endl;

	return 0;
}
