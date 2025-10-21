#pragma once
#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include <string>  // For std::string
#include <ctime>   // For time_t
#include <cstdlib> // For rand()
#include <iomanip> // For std::setfill and std::setw
#include <sstream> // For std::ostringstream
#include <iostream> // For std::cout


// Helper function to convert value to string
template <typename T>
std::string toString(T value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}


enum EBankAccountType { Checking, Saving, Credit, Pension, Loan };

class TBankAccount {

private:
	std::string accountNumber;
	EBankAccountType accountType;
	time_t creationTimestamp;
	double balance;

public:
	std::string ownerFirstName;
	std::string ownerLastName;

	//TBankAccount() {} // Don't use default constructor
	TBankAccount(EBankAccountType, std::string, std::string);
	~TBankAccount();

	std::string getAccountNumber() const;
	std::string getCreationTimeString() const;
	time_t getCreationTimestamp() const;
	double getBalance() const;
	void deposit(double);
	void withdraw(double);
	EBankAccountType getAccountType() const;
	std::string getAccountTypeString() const;
	void printAccountInfo() const;
};

#endif // BANKACCOUNT_H