// Submission-01.cpp : Defines the entry point for the application.
//

#include "main.h"
#include <fstream>
#include <iostream> 
#include <sstream> // For std::ostringstream
#include <string> // For std::getline and std::string

// Helper function to convert value to string
template <typename T>
std::string toString(T value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

/*
Part 1: The Banking System Foundation
First, you'll set up the core data structures for our banking application. This includes the data class itself and the linked list that will manage it.

EBankAccountType Enum:
Create an enum class called EBankAccountType.
It should include at least 5 different account types (e.g., Checking, Savings, Credit, Pension, Loan).
TBankAccount Class:
Create a class called TBankAccount to represent a bank account.
The class must contain: accountNumber (string), accountType (EBankAccountType), ownerFirstName (string), ownerLastName (string), and creationTimestamp (time_t), balance (float/double)
balance - set random from: if Checking/Debit/Savings/Pension 0 to 1000, if Loan -50000 to -25000, if Credit -1000 to 0
Implement a suitable constructor to initialize all data fields.
The TLinkedList Class & Your First Design Choice:
Create a generic TLinkedList class that will hold pointers to your TBankAccount objects.
Your Decision: You must decide whether to implement this as a singly-linked, doubly-linked, or circular list. In your final report, you must include a paragraph justifying your choice.
The class must include a boolean data field, ownsData. The constructor should set this flag.
The destructor (~TLinkedList()) must check the ownsData flag to correctly manage the memory of the TBankAccount objects and the list nodes.
*/

// deligate a function for callback to read names from file
typedef bool (*FNameRead)(const std::string& firstName, const std::string& lastName);

static void readNamesFromFile(const std::string& aFilename, FNameRead aOnNameRead)
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



enum EBankAccountType { Checking, Saving, Credit, Pension, Loan };


static EBankAccountType getRandomAccountType()
{
	return static_cast<EBankAccountType>(rand() % 5); // Randomly returns one of the 5 account types
}

class TBankAccount
{
private:
	std::string accountNumber;
	EBankAccountType accountType;
	time_t creationTimestamp;
	double balance;

public:
	std::string ownerFirstName;
	std::string ownerLastName;

	//TBankAccount() {}

	TBankAccount(EBankAccountType accType, std::string firstName, std::string lastName)
		: accountType(accType), ownerFirstName(firstName), ownerLastName(lastName)
	{
		// Random genration of account number: XXXX.XX.XXXXX
		accountNumber = toString(rand() % 9000 + 1000) + "." + toString(rand() % 90 + 10) + "." + toString(rand() % 90000 + 10000);

		balance = 0.0f;

		//Random generation of creation timestamp, date is any date and time in 2024
		int month = rand() % 12 + 1;
		int day = rand() % 28 + 1; // To avoid complexity of different month lengths
		int hour = rand() % 24;
		int minute = rand() % 60;

		// Calculate creation timestamp in seconds from 2024-01-01 00:00:00
	std:tm tm = {};
		tm.tm_year = 2024 - 1900; // Year since 1900
		tm.tm_mon = rand() % 12; // Month [0-11]
		tm.tm_mday = rand() % 28 + 1; // Day of the month [1-28] to avoid month length issues
		tm.tm_hour = rand() % 24; // Hour [0-23]
		tm.tm_min = rand() % 60; // Minute [0-59]
		tm.tm_sec = 0; // Second [0-59]
		creationTimestamp = _mkgmtime(&tm); // Use _mkgmtime for UTC

		if (accType == Checking || accType == Saving || accType == Pension)
			balance = static_cast<double>(rand() % 1001); // 0 to 1000
		else if (accType == Loan)
			balance = static_cast<double>(-(rand() % 25001 + 25000)); // -50000 to -25000
		else if (accType == Credit)
			balance = static_cast<double>(-(rand() % 1001)); // -1000 to 0
	}

	~TBankAccount()
	{
		// Destructor logic if needed
	}

	std::string getAccountNumber() const { return accountNumber; }
	EBankAccountType getAccountType() const { return accountType; }
	time_t getCreationTimestamp() const { return creationTimestamp; }
	double getBalance() const { return balance; }
	void deposit(double aAmount) { if (aAmount > 0) balance += aAmount; }
	void withdraw(double aAmount) { if (aAmount > 0 && aAmount <= balance) balance -= aAmount; }
	std::string getAccountTypeString() const
	{
		switch (accountType)
		{
		case Checking: return "Checking";
		case Saving: return "Saving";
		case Credit: return "Credit";
		case Pension: return "Pension";
		case Loan: return "Loan";
		default: return "Unknown";
		}
	}

	//
	std::string getCreationTimeString() const
	{
		char buffer[26];
		ctime_s(buffer, sizeof(buffer), &creationTimestamp);
		std::string timeString(buffer);
		if (!timeString.empty() && timeString.back() == '\n') {
			timeString.pop_back(); // Remove the trailing newline character
		}
		return timeString;
	}

	void printAccountInfo() const
	{
		std::cout << "Account Number: " << accountNumber << ", Type: " << getAccountTypeString()
			<< ", Owner: " << ownerFirstName << " " << ownerLastName
			<< ", Balance: " << balance
			<< ", Created: " << getCreationTimeString()
			<< std::endl;
	}

};


// For statistics
typedef struct _TSummary {
	long comparisonCount = 0;
	double timeTaken = 0.0;
}TSummary;
static TSummary statistics;

typedef bool (*FCompareAccount)(TBankAccount* account, void* searchKey);

// Node class for linked list
class TLinkedListNode
{
public:
	TBankAccount* data;
	TLinkedListNode* next;
	TLinkedListNode(TBankAccount* aData) : data(aData), next(nullptr) {}
	~TLinkedListNode()
	{
		// Destructor logic if needed
	}
};

// Use dummy head node for simplicity
class TLinkedList
{
private:
	TLinkedListNode* head;
	bool ownsData;
	int size;
public:
	TLinkedList(bool aOwnsData) : head(nullptr), ownsData(aOwnsData), size(0) {
		head = new TLinkedListNode(nullptr); // Dummy head node
	}
	~TLinkedList()
	{
		while (head->next != nullptr)
		{
			TLinkedListNode* temp = head->next;
			head->next = temp->next;
			if (ownsData) delete temp->data; // Delete the TBankAccount object
			delete temp; // Delete the node
		}
		delete head;
	}

	int getSize() const { return size; }

	void Add(TBankAccount* aData)
	{
		TLinkedListNode* newNode = new TLinkedListNode(aData);
		newNode->next = head->next;
		head->next = newNode;
		size++;
	}

	TBankAccount* Find(FCompareAccount aCompareFunc, void* aSearchKey)
	{
		TLinkedListNode* current = head->next;
		while (current != nullptr)
		{
			statistics.comparisonCount++;
			if (aCompareFunc(current->data, aSearchKey))
			{
				return current->data; // Found
			}
			current = current->next;
		}
		return nullptr; // Not found
	}

	TLinkedList* Every(FCompareAccount aCompareFunc, void* aSearchKey)
	{
		TLinkedList* resultList = new TLinkedList(false); // New list does not own data
		TLinkedListNode* current = head->next;
		while (current != nullptr)
		{
			statistics.comparisonCount++;
			if (aCompareFunc(current->data, aSearchKey))
			{
				resultList->Add(current->data); // Add to result list
			}
			current = current->next;
		}
		return resultList; // Return the new list
	}

	TBankAccount** ToArray()
	{
		if (size == 0) return nullptr;
		TBankAccount** array = new TBankAccount * [size];
		TLinkedListNode* current = head->next;
		int index = 0;
		while (current != nullptr && index < size) // Ensure index < size
		{
			array[index++] = current->data;
			current = current->next;
		}
		return array;
	}

	void forEach(void(*aFunc)(TBankAccount* account, int aIndex))
	{
		TLinkedListNode* current = head->next;
		int index = 0;
		while (current != nullptr)
		{
			aFunc(current->data, index++);
			current = current->next;
		}
	}
};

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
