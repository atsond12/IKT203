#include "BankAccount.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib> // For rand()
#include <random> // For better random number generation
#include <ctime>
#include <cstring> // For memset
#include <cmath>   // For floor
#include <chrono>  // For time manipulation
#include <locale>  // For locale settings
#include <codecvt> // For codecvt_utf8
#include <stdexcept> // For std::invalid_argument
#include <string>


TBankAccount::TBankAccount(EBankAccountType accType, std::string firstName, std::string lastName)
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

TBankAccount::~TBankAccount()
{
	// Destructor logic if needed
}

std::string TBankAccount::getAccountNumber() const {
	return accountNumber;
}

EBankAccountType TBankAccount::getAccountType() const {
	return accountType;
}

time_t TBankAccount::getCreationTimestamp() const {
	return creationTimestamp;
}
double TBankAccount::getBalance() const {
	return balance;
}
void TBankAccount::deposit(double aAmount) {
	if (aAmount > 0) balance += aAmount; 
}

void TBankAccount::withdraw(double aAmount) {
	if (aAmount > 0 && aAmount <= balance) balance -= aAmount; 
}

std::string TBankAccount::getAccountTypeString() const
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
std::string TBankAccount::getCreationTimeString() const
{
	char buffer[26];
	ctime_s(buffer, sizeof(buffer), &creationTimestamp);
	std::string timeString(buffer);
	if (!timeString.empty() && timeString.back() == '\n') {
		timeString.pop_back(); // Remove the trailing newline character
	}
	return timeString;
}

void TBankAccount::printAccountInfo() const
{
	std::cout << "Account Number: " << accountNumber << ", Type: " << getAccountTypeString()
		<< ", Owner: " << ownerFirstName << " " << ownerLastName
		<< ", Balance: " << balance
		<< ", Created: " << getCreationTimeString()
		<< std::endl;
}