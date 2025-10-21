// Submission-01.cpp : Defines the entry point for the application.
//

#include "main.h"
#include "ReadNames.h"		// For reading names from file
#include "BankAccount.h"	// For TBankAccount and EBankAccountType
#include "BankAccountList.h" // For TLinkedList
#include <string>					// For std::getline and std::string
#include <iostream>				// For std::cout
#include <sstream>				// For std::istringstream



// For statistics
typedef struct _TSummary {
	long comparisonCount = 0;
	double timeTaken = 0.0;
}TSummary;
static TSummary statistics;



static EBankAccountType getRandomAccountType()
{
	return static_cast<EBankAccountType>(rand() % 5); // Randomly returns one of the 5 account types
}

TLinkedList* bankAccounts = new TLinkedList(true); // List owns the TBankAccount objects
TBankAccount** bankAccountArray = nullptr;


static bool OnNameRead(const std::string& firstName, const std::string& lastName)
{
	//For each name read, create from 5 to 10 random bank accounts
	int accountCount = rand() % 6 + 5; // Random number between 5 and 10
	for (int i = 0; i < accountCount; i++)
	{
		EBankAccountType accType = getRandomAccountType();
		TBankAccount* newAccount = new TBankAccount(accType, firstName, lastName);
		bankAccounts->Add(newAccount);
	}
	return true; //bankAccounts->getSize() < 100; // For demo purposes
}

static void resetStatistics()
{
	statistics.comparisonCount = 0;
	statistics.timeTaken = (static_cast<double>(clock())) / CLOCKS_PER_SEC;
}

static void printStastics() {
	statistics.timeTaken = (static_cast<double>(clock())) / CLOCKS_PER_SEC - statistics.timeTaken;
	std::cout << "Comparisons: " << statistics.comparisonCount << ", Time taken: " << statistics.timeTaken << " seconds." << std::endl;
}

/*
Part 3: Standalone Search Functions (The External Analyst)
To simulate working with data from different perspectives, you will also implement search functions that are not part of the list class. These functions will operate on a simple array of pointers.
*/

static TBankAccount* FindAccountByNumber(TBankAccount** accountArray, int arraySize, const std::string& accountNumber) {
	if (accountArray == nullptr || arraySize <= 0) return nullptr;
	for (int i = 0; i < arraySize; i++) {
		statistics.comparisonCount++;
		if (accountArray[i]->getAccountNumber() == accountNumber) {
			return accountArray[i]; // Found
		}
	}
	return nullptr; // Not found
}

static void PrintEveryAccountInDateRange(TBankAccount** accountArray, int arraySize, time_t from, time_t to) {
	if (accountArray == nullptr || arraySize <= 0) return;
	std::cout << "------------------------------" << std::endl;
	resetStatistics();
	int foundCount = 0;
	for (int i = 0; i < arraySize; i++) {
		statistics.comparisonCount++;
		time_t ts = accountArray[i]->getCreationTimestamp();
		if (ts >= from && ts < to) {
			std::cout << i + 1 << ". ";
			accountArray[i]->printAccountInfo();
			foundCount++;
		}
	}
	printStastics();
	std::cout << "Total accounts found in date range: " << foundCount << std::endl;
}


int main()
{
	std::cout << "--- Submission 4: Sosrt & Search ---" << std::endl;

	// Test TBankAccount
	//Gen random account type
	//Change this name for you own names file
	std::string namesFile = "F:\\IKT203\\VisualStudio\\DATA\\Random_Name.txt";
	std::cout << "Reading names from file: " << namesFile << std::endl;
	readNamesFromFile(namesFile, OnNameRead);
	std::cout << "Total Bank Accounts Created: " << bankAccounts->getSize() << std::endl;
	std::cout << "Converting linked list to array..." << std::endl;
	bankAccountArray = bankAccounts->ToArray();
	std::cout << "Array created with " << bankAccounts->getSize() << " accounts." << std::endl;


	resetStatistics();
	int getRandomIndex = rand() % bankAccounts->getSize();
	TBankAccount* foundAccount = FindAccountByNumber(bankAccountArray, bankAccounts->getSize(), bankAccountArray[getRandomIndex]->getAccountNumber());
	if (foundAccount)
	{
		std::cout << "Found Account: " << std::endl;
		foundAccount->printAccountInfo();
	}
	else
	{
		std::cout << "Account not found." << std::endl;
	}
	printStastics();

	resetStatistics();
	foundAccount = FindAccountByNumber(bankAccountArray, bankAccounts->getSize(), "1234.56.78901");
	if (foundAccount)
	{
		std::cout << "Found Account: " << std::endl;
		foundAccount->printAccountInfo();
	}
	else
	{
		std::cout << "Account not found." << std::endl;
	}
	printStastics();

	// Find All (Integrated): Use your Every() method to find all accounts created in June 2024 and print their details.
	resetStatistics();
	struct June2024Key {
		time_t start;
		time_t end;
	};
	June2024Key juneKey{};
	std::tm fromToTm = {};
	fromToTm.tm_year = 2024 - 1900; // Year since 1900
	fromToTm.tm_mon = 5; // June (0-based)
	fromToTm.tm_mday = 1; // 1st
	fromToTm.tm_hour = 0;
	fromToTm.tm_min = 0;
	fromToTm.tm_sec = 0;
	juneKey.start = _mkgmtime(&fromToTm); // Use _mkgmtime for UTC
	fromToTm.tm_mday = 30; // 30th
	fromToTm.tm_hour = 23;
	fromToTm.tm_min = 59;
	fromToTm.tm_sec = 59;
	juneKey.end = _mkgmtime(&fromToTm); // Use _mkgmtime for UTC

	TLinkedList* juneAccounts = bankAccounts->Every(
		[](TBankAccount* account, void* searchKey) -> bool {
			June2024Key* key = static_cast<June2024Key*>(searchKey);
			time_t ts = account->getCreationTimestamp();
			return ts >= key->start && ts < key->end;
		}, &juneKey);

	std::cout << "Accounts created in June 2024: " << juneAccounts->getSize() << std::endl;
	printStastics();

	juneAccounts->forEach(
		[](TBankAccount* aAccount, int aIndex) {
			std::cout << aIndex + 1 << ". ";
			aAccount->printAccountInfo();
		});

	PrintEveryAccountInDateRange(bankAccountArray, bankAccounts->getSize(), juneKey.start, juneKey.end);


	// Cleanup
	// First delete the array, then the linked list
	delete[] bankAccountArray;
	delete bankAccounts;

	return 0;
}
