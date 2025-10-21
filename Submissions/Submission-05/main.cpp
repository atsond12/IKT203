// Submission-01.cpp : Defines the entry point for the application.
//

#include "ReadNames.h"		// For reading names from file
#include "BankAccount.h"	// For TBankAccount and EBankAccountType
#include "BankAccountList.h" // For TLinkedList
#include <string>					// For std::getline and std::string
#include <iostream>				// For std::cout
#include <sstream>				// For std::istringstream



// For statistics
typedef struct _TSummary {
	long long comparisonCount = 0;
	long long swaps = 0;
	double timeTaken = 0.0;
}TSummary;
static TSummary statistics;


static void resetStatistics()
{
	statistics.comparisonCount = 0;
	statistics.swaps = 0;
	statistics.timeTaken = (static_cast<double>(clock())) / CLOCKS_PER_SEC;
}

static void printStastics() {
	statistics.timeTaken = (static_cast<double>(clock())) / CLOCKS_PER_SEC - statistics.timeTaken;
	std::cout << "Comparisons: " << statistics.comparisonCount << ", Swaps: " << statistics.swaps << ", Time taken : " << statistics.timeTaken << " seconds." << std::endl;
}


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
	return bankAccounts->getSize() < 2500; // For demo purposes
}

/*
Part 1: The Sorting Toolkit
Before we can sort, we need the right tools. In this part, you'll set up a flexible, powerful sorting "engine" that can handle any sorting criteria we give it.

1. The FCompareAccounts Callback:
	a) Create a typedef for a function pointer named FCompareAccounts.
	b) The signature must be: int (*FCompareAccounts)(TBankAccount* a, TBankAccount* b);.
	c) This function should return a negative value if a comes before b, zero if they are equal, and a positive value if a comes after b.
2. The OperationSummary Struct:
	a) Create a struct named OperationSummary to track performance metrics: long long comparisons, long long swaps, and double timeSpentMs.
3. The TSort Class:
	a) Create a class called TSort. This will be your dedicated sorting engine.
	b) The constructor should take pointers to the original data sources (the list and the array).
	c) The sorting methods should create and return a new, sorted array or list, not modify the original.
*/

/*
Part 2: The Simple Sorts (O(n²)) - Foundational, but Slow
These algorithms are your first step. They are conceptually simpler but do not perform well on large datasets. Implementing them is essential for understanding the fundamentals.

4. Selection Sort:
	a) The Challenge: You must implement this algorithm twice in your TSort class:
		1. A version that sorts the pointer array.
		2. A version that sorts the linked list.
	b) Pay close attention to the pointer manipulation required for the linked list version—it's a fantastic challenge!
5. Bubble Sort:
	a) Implement a method in TSort that performs a Bubble Sort on the pointer array.
*/

typedef int (*FCompareAccounts)(TBankAccount*, TBankAccount*);

class TSort
{
private:
	TLinkedList* list;
	TBankAccount** array;
	int size;
	void swap(TBankAccount* a, TBankAccount* b) {
		TBankAccount* temp = a;
		a = b;
		b = temp;
		statistics.swaps++;
	}
public:
	TSort(TLinkedList* aList, TBankAccount** aArray) : list(aList), array(aArray) {
		size = list->getSize();
	}
	~TSort() {}

	TBankAccount** SelectionSortArray(FCompareAccounts compare) {
		resetStatistics();
		TBankAccount** sortedArray = new TBankAccount * [size];
		for (int i = 0; i < size; i++) {
			sortedArray[i] = array[i];
		}
		for (int i = 0; i < size - 1; i++) {
			int minIndex = i;
			for (int j = i + 1; j < size; j++) {
				statistics.comparisonCount++;
				if (compare(sortedArray[j], sortedArray[minIndex]) < 0) {
					minIndex = j;
				}
			}
			if (minIndex != i) {
				swap(sortedArray[i], sortedArray[minIndex]);
				TBankAccount* temp = sortedArray[i];
				sortedArray[i] = sortedArray[minIndex];
				sortedArray[minIndex] = temp;
			}
		}
		printStastics();
		return sortedArray;
	}

	TLinkedList* SelectionSortList(FCompareAccounts aCompareFunc) {
		resetStatistics();
		TLinkedList* sortedList = new TLinkedList(true);
		TLinkedList* tempList = new TLinkedList(false);
		for (int i = 0; i < size; i++) {
			tempList->Add(array[i]);
		}
		while (tempList->getSize() > 0) {
			TLinkedListNode* current = tempList->getHead();
			TLinkedListNode* minNode = current;
			if (!minNode) break; // Prevent dereferencing nullptr
			while (current) {
				statistics.comparisonCount++;
				if (aCompareFunc(current->data, minNode->data) < 0) {
					minNode = current;
				}
				current = current->next;
			}
			// Fix: Only delete minNode if it's not nullptr and not the same as current
			if (minNode) {
				sortedList->Add(minNode->data);
				tempList->Remove(minNode->data);
				// Do not delete minNode here, as Remove already deletes the node if needed
				statistics.swaps++;
			}
		}
		delete tempList;
		printStastics();
		return sortedList;
	}
	// Bubble Sort for array, Time Complexity O(n^2), space O(1)
	TBankAccount** BubbleSortArray(FCompareAccounts compare) {
		resetStatistics();
		TBankAccount** sortedArray = new TBankAccount * [size];
		for (int i = 0; i < size; i++) {
			sortedArray[i] = array[i];
		}
		for (int i = 0; i < size - 1; i++) {
			for (int j = 0; j < size - i - 1; j++) {
				statistics.comparisonCount++;
				if (compare(sortedArray[j], sortedArray[j + 1]) > 0) {
					swap(sortedArray[j], sortedArray[j + 1]);
					TBankAccount* temp = sortedArray[j];
					sortedArray[j] = sortedArray[j + 1];
					sortedArray[j + 1] = temp;
				}
			}
		}
		printStastics();
		return sortedArray;
	}

	// Bubble Sort for linked list, Time Complexity O(n^2), space O(1)
	TLinkedList* BubbleSortList(FCompareAccounts aCompareFunc) {
		resetStatistics();
		TLinkedList* sortedList = new TLinkedList(true);
		TLinkedList* tempList = new TLinkedList(false);
		for (int i = 0; i < size; i++) {
			tempList->Add(array[i]);
		}
		int n = tempList->getSize();
		for (int i = 0; i < n - 1; i++) {
			TLinkedListNode* current = tempList->getHead();
			TLinkedListNode* nextNode = current ? current->next : nullptr;
			for (int j = 0; j < n - i - 1; j++) {
				if (!current || !nextNode) break; // Prevent dereferencing nullptr
				statistics.comparisonCount++;
				if (aCompareFunc(current->data, nextNode->data) > 0) {
					swap(current->data, nextNode->data);
					TBankAccount* temp = current->data;
					current->data = nextNode->data;
					nextNode->data = temp;
				}
				current = nextNode;
				nextNode = nextNode->next;
			}
		}
		// Transfer sorted data to sortedList
		TLinkedListNode* current = tempList->getHead();
		while (current) {
			sortedList->Add(current->data);
			current = current->next;
		}
		delete tempList;
		printStastics();
		return sortedList;
	}
	
};

// Comparison functions
// Comapre based on account number
static int CompareByAccountNumber(TBankAccount* a, TBankAccount* b) {
	return a->getAccountNumber().compare(b->getAccountNumber());
}

//Cmpare based on creation timestamp
static int CompareByCreationTimestamp(TBankAccount* a, TBankAccount* b) {
	if (a->getCreationTimestamp() < b->getCreationTimestamp()) return -1;
	if (a->getCreationTimestamp() > b->getCreationTimestamp()) return 1;
	return 0;
}

/*
Part 3: The Advanced Sorts (O(n log n)) - Divide and Conquer
Now for the heavy hitters. These recursive, "Divide and Conquer" algorithms are far more efficient and are staples of modern software engineering.

6. Quick Sort (on the Array):
	a) Implement Quick Sort to sort the pointer array.
	b) Your implementation must use the public/private recursion pattern. A public QuickSort() method calls a private QuickSortRecursive(...).
	c) The heart of this algorithm is the Partition() helper function. Getting this right is the key to success!

7. Merge Sort (on the Linked List):
	a) Implement Merge Sort to sort the linked list. This algorithm is a natural fit for list structures.
	b) This implementation must also use the public/private recursion pattern.
	c) Hint: For splitting the linked list, research the "fast and slow pointer" technique.

Part 4: The Great Sort-Off
It's time for a performance battle! You will use your TSort engine to sort the same large dataset with all your implemented algorithms and analyze the results.

8. Callback Implementations:
	a) Write at least two different FCompareAccounts callback functions: one to sort by last name, and one to sort by balance.

9. The Performance Battle:
	a) Using your dataset of 5,000+ accounts, run all four of your sorting algorithms using the same callback function for a fair comparison.
	b) For each run, capture the OperationSummary (comparisons, swaps, time).

10. Analysis in Your Report:
	a) Present your performance data in a clear table.
	b) Write a paragraph answering: How do the results illustrate the difference between O(n²) and O(n log n) complexity? Why was Selection Sort harder on a list versus an array?
*/




int main()
{
	std::cout << "--- Submission 5: The Algorithmic Organizer ---" << std::endl;
	std::string namesFile = "F:\\IKT203\\VisualStudio\\DATA\\Random_Name.txt";
	std::cout << "Reading names from file: " << namesFile << std::endl;
	readNamesFromFile(namesFile, OnNameRead);
	std::cout << "Total Bank Accounts Created: " << bankAccounts->getSize() << std::endl;
	std::cout << "Converting linked list to array..." << std::endl;
	bankAccountArray = bankAccounts->ToArray();
	std::cout << "Array created with " << bankAccounts->getSize() << " accounts." << std::endl;

	TSort sorter(bankAccounts, bankAccountArray);
	sorter.SelectionSortArray(CompareByAccountNumber);
	//sorter.SelectionSortList(CompareByCreationTimestamp);
	//sorter.BubbleSortArray(CompareByAccountNumber);
	//sorter.BubbleSortList(CompareByCreationTimestamp);


	// Cleanup
	// First delete the array, then the linked list
	delete[] bankAccountArray;
	delete bankAccounts;

	return 0;
}
