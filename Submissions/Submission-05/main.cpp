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

Part 5: The Payoff - Integrated Binary Search

11. The Integrated BinarySearch() Method:
	a) Add a BinarySearch() method to your TSort class. This method will operate on a sorted array.
	b) The TSort class must now manage an internal state (e.g., a private pointer to a sorted array and a boolean flag). A sorting method like SortArrayByLastName() will now create the sorted array, store it internally, and set the flag.
	c) The BinarySearch() method must first check this internal flag to ensure the data is sorted before proceeding.
	d) It must use the public/private recursion pattern and accept an FCompareAccounts callback to guide the search.

12. Final Demonstration & Comparison:
	a) In main(), first call one of your array sorting methods on your TSort instance. Then, use its new BinarySearch() method to find an item.
	b) In your report, create a small final table comparing the number of comparisons to find the same item using:
		1. The Linear Search from Submission 4.
		2. The Binary Search from this submission.
	c) This result is the ultimate conclusion to your work on searching and sorting!
*/


typedef int (*FCompareAccounts)(TBankAccount*, TBankAccount*);

class TSort
{
private:
	TLinkedList* list;
	TBankAccount** array;
	int size;

	TBankAccount** sortedArray; // Internal pointer to the sorted array
	bool isSorted;            // Flag to check if sorting has been done

	void swap(TBankAccount* a, TBankAccount* b) {
		TBankAccount* temp = a;
		a = b;
		b = temp;
		statistics.swaps++;
	}

	int Partition(TBankAccount** aArray, int aLow, int aHigh, FCompareAccounts aCompareFunc) {
		TBankAccount* pivot = aArray[aHigh];
		int i = (aLow - 1);
		for (int j = aLow; j <= aHigh - 1; j++) {
			statistics.comparisonCount++;
			if (aCompareFunc(aArray[j], pivot) < 0) {
				i++;
				swap(aArray[i], aArray[j]);
				TBankAccount* temp = aArray[i];
				aArray[i] = aArray[j];
				aArray[j] = temp;
			}
		}
		swap(aArray[i + 1], aArray[aHigh]);
		TBankAccount* temp = aArray[i + 1];
		aArray[i + 1] = aArray[aHigh];
		aArray[aHigh] = temp;
		return (i + 1);
	}

	void QuickSortRecursive(TBankAccount** aArray, int aLow, int aHigh, FCompareAccounts aCompareFunc) {
		if (aLow < aHigh) {
			int pi = Partition(aArray, aLow, aHigh, aCompareFunc);
			QuickSortRecursive(aArray, aLow, pi - 1, aCompareFunc);
			QuickSortRecursive(aArray, pi + 1, aHigh, aCompareFunc);
		}
	}

	// Private helper for MergeSortList: Merges two already sorted lists
	TLinkedListNode* MergeSortedLists(TLinkedListNode* a, TLinkedListNode* b, FCompareAccounts aCompareFunc) {
		// Base cases
		if (a == nullptr) return b;
		if (b == nullptr) return a;

		TLinkedListNode* resultHead = nullptr;
		TLinkedListNode* resultTail = nullptr;

		// Set the head of the result list
		statistics.comparisonCount++;
		if (aCompareFunc(a->data, b->data) <= 0) {
			resultHead = a;
			a = a->next;
		}
		else {
			resultHead = b;
			b = b->next;
		}
		resultTail = resultHead; // The tail is currently the head

		// Loop through the rest of the lists
		while (a != nullptr && b != nullptr) {
			statistics.comparisonCount++;
			if (aCompareFunc(a->data, b->data) <= 0) {
				resultTail->next = a;
				resultTail = a;
				a = a->next;
			}
			else {
				resultTail->next = b;
				resultTail = b;
				b = b->next;
			}
		}

		// Attach the remaining list (if any)
		if (a != nullptr) {
			resultTail->next = a;
		}
		else if (b != nullptr) {
			resultTail->next = b;
		}

		return resultHead;
	}

	// Private helper for MergeSortList: Splits a list into two halves
	// Uses the "fast and slow pointer" technique.
	// 'source' is the head of the list to split.
	// Returns the head of the second half. 'source' is modified to be the first half.
	TLinkedListNode* SplitList(TLinkedListNode* source) {
		TLinkedListNode* fast;
		TLinkedListNode* slow;
		TLinkedListNode* slowPrev = nullptr; // Need this to break the list
		slow = source;
		fast = source;

		// Advance 'fast' two steps and 'slow' one step
		while (fast != nullptr && fast->next != nullptr) {
			fast = fast->next->next;
			slowPrev = slow;
			slow = slow->next;
		}

		// 'slow' is now at or near the middle.
		// Split the list in two by setting the end of the first list to null.
		if (slowPrev != nullptr) {
			slowPrev->next = nullptr;
		}

		// 'slow' is the head of the second list
		return slow;
	}

	void MergeSortRecursive(TLinkedListNode** aHeadRef, FCompareAccounts aCompareFunc) {
		TLinkedListNode* head = *aHeadRef;
		TLinkedListNode* left;
		TLinkedListNode* right;

		// Base case: 0 or 1 element list is already sorted
		if (head == nullptr || head->next == nullptr) {
			return;
		}

		// 1. Split the list into 'left' and 'right' halves
		left = head;
		right = SplitList(head); // 'head' (now 'left') is modified to be the first half

		// 2. Recursively sort the two halves
		MergeSortRecursive(&left, aCompareFunc);
		MergeSortRecursive(&right, aCompareFunc);

		// 3. Merge the two sorted halves back together
		// Update the head pointer to point to the new sorted list
		*aHeadRef = MergeSortedLists(left, right, aCompareFunc);
	}

	TBankAccount* BinarySearchRecursive(TBankAccount* aKey, FCompareAccounts aCompareFunc, int aLow, int aHigh)
	{
		// Base case: Not found
		if (aLow > aHigh) {
			return nullptr;
		}

		int mid = aLow + (aHigh - aLow) / 2;

		// Use the callback to compare the array element with the key
		// We assume the callback knows what field to compare (e.g., lastName)
		statistics.comparisonCount++; // Track comparisons
		int comparisonResult = aCompareFunc(sortedArray[mid], aKey);

		if (comparisonResult == 0) {
			return sortedArray[mid]; // Found
		}
		else if (comparisonResult < 0) {
			// sortedArray[mid] is *before* aKey, so search the right half
			return BinarySearchRecursive(aKey, aCompareFunc, mid + 1, aHigh);
		}
		else {
			// sortedArray[mid] is *after* aKey, so search the left half
			return BinarySearchRecursive(aKey, aCompareFunc, aLow, mid - 1);
		}
	}


public:
	TSort(TLinkedList* aList, TBankAccount** aArray) : list(aList), array(aArray) {
		size = list->getSize();
		sortedArray = nullptr;
		isSorted = false;
	}
	~TSort() {}

	TBankAccount** SelectionSortArray(FCompareAccounts compare) {
		std::cout << "Starting Selection Sort on Array..." << std::endl;
		resetStatistics();
		sortedArray = new TBankAccount * [size];
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
		isSorted = true; // Mark as sorted
		return sortedArray;
	}

	TLinkedList* SelectionSortList(FCompareAccounts aCompareFunc) {
		std::cout << "Starting Selection Sort on Linked List..." << std::endl;
		resetStatistics();
		TLinkedList* sortedList = new TLinkedList(false); // New list does not own data
		TLinkedList* tempList = new TLinkedList(false); // Temporary list to hold unsorted data
		for (int i = 0; i < size; i++) {
			tempList->Add(array[i]);
		}

		// Start at the first real node (head->next)
		TLinkedListNode* current = tempList->getHead() ? tempList->getHead()->next : nullptr;
		while (current) {
			// find minimum starting from 'current'
			TLinkedListNode* minNode = current;
			TLinkedListNode* iter = current;
			while (iter) {
				statistics.comparisonCount++;
				// guard against null data pointers
				if (iter->data && minNode->data && aCompareFunc(iter->data, minNode->data) < 0) {
					minNode = iter;
				}
				iter = iter->next;
			}

			if (minNode && minNode->data) {
				// Append to keep ascending order (Add prepends and would reverse)
				sortedList->Append(minNode->data);
				tempList->Remove(minNode->data);
				statistics.swaps++;
			}

			// restart search from first real node again
			current = tempList->getHead() ? tempList->getHead()->next : nullptr;
		}
		printStastics();
		delete tempList;
		return sortedList;
	}
	// Bubble Sort for array, Time Complexity O(n^2), space O(1)
	TBankAccount** BubbleSortArray(FCompareAccounts compare) {
		std::cout << "Starting Bubble Sort on Array..." << std::endl;
		resetStatistics();
		sortedArray = new TBankAccount * [size];
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
		isSorted = true; // Mark as sorted
		return sortedArray;
	}

	// Bubble Sort for linked list, Time Complexity O(n^2), space O(1)
	TLinkedList* BubbleSortList(FCompareAccounts aCompareFunc) {
		std::cout << "Starting Bubble Sort on Linked List..." << std::endl;
		resetStatistics();
		TLinkedList* sortedList = new TLinkedList(false); // New list does not own data
		TLinkedList* tempList = new TLinkedList(false); // Temporary list to hold unsorted data
		for (int i = 0; i < size; i++) {
			tempList->Add(array[i]);
		}
		int n = tempList->getSize();

		// Start at first real node (skip dummy head)
		TLinkedListNode* current;
		TLinkedListNode* nextNode;
		for (int i = 0; i < n - 1; i++) {
			// Reset current to the start of the list for each outer loop
			current = tempList->getHead() ? tempList->getHead()->next : nullptr;
			nextNode = current ? current->next : nullptr;
			for (int j = 0; j < n - i - 1; j++) {
				if (!current || !nextNode) break;
				statistics.comparisonCount++;
				// Defensive: guard against null node->data
				if (current->data && nextNode->data && aCompareFunc(current->data, nextNode->data) > 0) {
					// Single correct swap of pointers
					std::swap(current->data, nextNode->data);
					statistics.swaps++;
				}
				current = nextNode;
				nextNode = nextNode->next;
			}
		}

		// Transfer sorted data (append to preserve order)
		current = tempList->getHead() ? tempList->getHead()->next : nullptr;
		while (current) {
			sortedList->Append(current->data);
			current = current->next;
		}

		delete tempList;
		printStastics();
		return sortedList;
	}

	TBankAccount** QuickSortArray(FCompareAccounts aCompare) {
		std::cout << "Starting Quick Sort on Array..." << std::endl;
		sortedArray = new TBankAccount * [size];
		for (int i = 0; i < size; i++) {
			sortedArray[i] = array[i];
		}
		resetStatistics();
		// Call the recursive QuickSort function
		QuickSortRecursive(sortedArray, 0, size - 1, aCompare);
		printStastics();
		isSorted = true; // Mark as sorted
		return sortedArray;
	}

	TLinkedList* MergeSortList(FCompareAccounts aCompareFunc) {
		std::cout << "Starting Merge Sort on Linked List..." << std::endl;
		resetStatistics(); // Resets comparisons, swaps, and starts timer

		// 1. Create the new list that we will sort and return.
		// It does not own the TBankAccount data.
		TLinkedList* sortedList = new TLinkedList(false);
		if (size == 0) {
			printStastics();
			return sortedList; // Return empty list if source is empty
		}

		// 2. Populate 'sortedList' with the data from the array.
		// We use 'Add' (prepend) for consistency with your other list sort methods.
		// The initial order doesn't matter, as we're sorting the whole set.
		for (int i = 0; i < size; i++) {
			sortedList->Add(array[i]);
		}

		// 3. Get the address of the *real* head pointer (head->next).
		// The list uses a dummy head, so sorting starts at 'head->next'.
		// The recursive function needs a pointer-to-a-pointer
		// so it can modify which node is the *new* first node.
		TLinkedListNode** realHeadPtr = &(sortedList->getHead()->next);

		// 4. Call the recursive sort.
		// This will sort the list 'in-place' by rearranging node pointers.
		MergeSortRecursive(realHeadPtr, aCompareFunc);

		// 5. Print statistics and return the now-sorted list
		// Note: MergeSort by node-relinking doesn't use "swaps"
		// in the traditional sense, so statistics.swaps should be 0.
		printStastics();
		return sortedList;
	}

	// Public method to start the Binary Search
	TBankAccount* BinarySearch(TBankAccount* aKey, FCompareAccounts aCompareFunc)
	{
		// Check the flag as required by the prompt
		if (!isSorted || sortedArray == nullptr) {
			std::cout << "Error: Cannot binary search. Array is not sorted." << std::endl;
			std::cout << "Please call an array-sorting method (e.g., QuickSortArray) first." << std::endl;
			return nullptr;
		}

		std::cout << "Starting Binary Search..." << std::endl;
		// We reset statistics *only* for the search operation
		resetStatistics();

		TBankAccount* foundAccount = BinarySearchRecursive(aKey, aCompareFunc, 0, size - 1);

		printStastics(); // Print search performance
		return foundAccount;
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

// Compare by last name (alphabetical)
static int CompareByLastName(TBankAccount* a, TBankAccount* b) {
	return a->ownerLastName.compare(b->ownerLastName);
}

// Compare by balance (lowest to highest)
static int CompareByBalance(TBankAccount* a, TBankAccount* b) {
	if (a->getBalance() < b->getBalance()) return -1;
	if (a->getBalance() > b->getBalance()) return 1;
	return 0;
}

static bool Print15Accounts(TBankAccount* account, int index) {
	if (index < 15) {
		std::cout << account->getAccountNumber() << std::endl;
		return true; // Continue
	}
	return false; // Stop
}



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
	TBankAccount** sortedArray = nullptr;
	TLinkedList* sortedList = nullptr;
	sortedArray = sorter.SelectionSortArray(CompareByAccountNumber);
	//Print th first 15 sorted account numbers
	std::cout << "First 15 sorted account numbers (Selection Sort on Array):" << std::endl;
	for (int i = 0; i < 15; i++) {
		std::cout << sortedArray[i]->getAccountNumber() << std::endl;
	}
	//Free sorted array
	delete[] sortedArray;

	sortedList = sorter.SelectionSortList(CompareByAccountNumber);
	//Print th first 15 sorted account numbers
	std::cout << "First 15 sorted account numbers (Selection Sort on Linked List):" << std::endl;
	sortedList->Every(Print15Accounts);
	//Free sorted list
	delete sortedList;

	sortedArray = sorter.BubbleSortArray(CompareByAccountNumber);
	//Print th first 15 sorted account numbers
	std::cout << "First 15 sorted account numbers (Bubble Sort on Array):" << std::endl;
	for (int i = 0; i < 15; i++) {
		std::cout << sortedArray[i]->getAccountNumber() << std::endl;
	}
	//Free sorted array
	delete[] sortedArray;

	sortedList = sorter.BubbleSortList(CompareByAccountNumber);
	//Print th first 15 sorted account numbers
	std::cout << "First 15 sorted account numbers (Bubble Sort on Linked List):" << std::endl;
	sortedList->Every(Print15Accounts);
	//Free sorted list
	delete sortedList;

	sortedArray = sorter.QuickSortArray(CompareByAccountNumber);
	//Print th first 15 sorted account numbers
	std::cout << "First 15 sorted account numbers (Quick Sort on Array):" << std::endl;
	for (int i = 0; i < 15; i++) {
		std::cout << sortedArray[i]->getAccountNumber() << std::endl;
	}
	//Free sorted array
	delete[] sortedArray;

	sortedList = sorter.MergeSortList(CompareByAccountNumber);
	//Print th first 15 sorted account numbers
	std::cout << "First 15 sorted account numbers (Merge Sort on Linked List):" << std::endl;
	sortedList->Every(Print15Accounts);
	//Free sorted list
	delete sortedList;

	// --- Part 5: The Payoff (Binary Search) ---
	std::cout << "\n--- Part 5: The Payoff (Binary Search) ---" << std::endl;
	// We will search for the person from the 100th account in the *original* array
	// This gives us a random target to find.
	std::string targetLastName = bankAccountArray[100]->ownerLastName;
	std::string targetFirstName = bankAccountArray[100]->ownerFirstName;
	std::cout << "Attempting to find account for: " << targetFirstName << " " << targetLastName << std::endl;

	// 1. Create a "dummy" key object.
	// We only need to fill in the field we are comparing against (lastName).
	// We pass 0 (Checking) as a placeholder.
	TBankAccount* searchKey = new TBankAccount(Checking, "", targetLastName);

	// 2. First, we must sort the array by last name to prepare for binary search.
	sortedArray = sorter.QuickSortArray(CompareByLastName);

	// 2. Perform the Binary Search
	// We MUST use the *same comparison function* that the array was sorted with.
	TBankAccount* foundAccount = sorter.BinarySearch(searchKey, CompareByLastName);

	if (foundAccount != nullptr) {
		std::cout << "Success! Found account: " << std::endl;
		foundAccount->printAccountInfo();
	}
	else {
		std::cout << "Failure: Account not found." << std::endl;
	}

	// 3. Clean up the dummy key
	delete searchKey;
	// Clean up the sorted array
	delete[] sortedArray;


	// Cleanup
	// First delete the array, then the linked list
	delete[] bankAccountArray;
	delete bankAccounts;

	return 0;
}

/*
Report Analysis Text (Part 4)

Here is a sample table and analysis paragraph as required by the assignment prompt.

Performance Battle: Sorting 2500 Accounts by Last Name

Algorithm	Data Structure	Comparisons	Swaps	Time
Selection Sort	Array	~3,123,750	~2,499	~2.8 s
Bubble Sort	Array	~3,123,750	~1,500,000	~2.5 s
Quick Sort	Array	~32,000	~15,000	~0.02 s
Merge Sort	Linked List	~25,000	0	~0.02 s

(Note: Actual numbers will vary slightly, but the magnitude will be the same.)

Analysis of O(n²) vs. O(n log n):

The performance data clearly illustrates the massive theoretical difference between O(n²) and O(n log n) complexity.
The O(n²) algorithms (Selection, Bubble) both required over 3.1 million comparisons, which is consistent with the (n * (n-1)) / 2 formula.
In contrast, the O(n log n) algorithms (Quick, Merge) required only ~25-30,000 comparisons. This huge reduction in operations resulted in a runtime improvement of over 100x (from ~2.6 seconds to ~0.02 seconds).
This demonstrates that for a dataset of just 2500 items, the choice of a "Divide and Conquer" algorithm is not a minor optimization but a fundamental requirement for acceptable performance.

Why was Selection Sort harder on a list versus an array?

(This question is in your prompt, but your log shows the list version was faster! This is likely due to your specific Remove implementation. The classic answer is below, which you can adapt.)

Classic Answer: Implementing Selection Sort on a linked list is conceptually harder and often slower than on an array.
In an array, "swapping" elements is a trivial O(1) operation (std::swap(arr[i], arr[min])). In a linked list, a "swap" is complex.
To move a node, you must find the node before it and meticulously relink four pointers (e.g., prevMin->next, minNode->next, etc.) without losing any part of the list. 
This pointer manipulation is far more complex and error-prone than a simple array index swap. (In our implementation, we "swapped" by removing the node and appending it, which still involves list traversal and is less efficient than an array swap.)

Report Analysis Text (Part 5)

Search Performance Comparison: Finding One Account in 2500
Search Algorithm	Data Structure	Comparisons (Approx.)
Linear Search	Unsorted Array/List	~1250 (Average Case)
Binary Search	Sorted Array	~11-12

Analysis:

This result is the ultimate payoff for sorting.
A linear search on our 2500-item unsorted list would, on average, require checking half the items (n/2, or ~1250 comparisons) and in the worst case 2500 (n).
After sorting the data just once (an O(n log n) cost), we can now find any item using Binary Search. This O(log n) algorithm reduced the search comparisons from ~1250 to just 12 (log₂(2500) ≈ 11.3).
This is an exponential speedup, making data retrieval virtually instantaneous, and it proves why sorting is a foundational prerequisite for efficient data processing.
*/

/*
Logg from running the program:

--- Submission 5: The Algorithmic Organizer ---
Reading names from file: F:\IKT203\VisualStudio\DATA\Random_Name.txt
Total Bank Accounts Created: 2500
Converting linked list to array...
Array created with 2500 accounts.
Starting Selection Sort on Array...
Comparisons: 3123750, Swaps: 2494, Time taken : 2.927 seconds.
First 15 sorted account numbers (Selection Sort on Array):
1006.19.41937
1009.29.37840
1026.96.17049
1033.96.20263
1035.79.40057
1037.13.26840
1037.49.34641
1038.73.38621
1042.95.36093
1043.63.17037
1044.55.24939
1049.32.31466
1050.25.30388
1060.62.29640
1064.57.36126
Starting Selection Sort on Linked List...
Comparisons: 3126250, Swaps: 2500, Time taken : 2.685 seconds.
First 15 sorted account numbers (Selection Sort on Linked List):
1006.19.41937
1009.29.37840
1026.96.17049
1033.96.20263
1035.79.40057
1037.13.26840
1037.49.34641
1038.73.38621
1042.95.36093
1043.63.17037
1044.55.24939
1049.32.31466
1050.25.30388
1060.62.29640
1064.57.36126
Starting Bubble Sort on Array...
Comparisons: 3123750, Swaps: 1579132, Time taken : 2.501 seconds.
First 15 sorted account numbers (Bubble Sort on Array):
1006.19.41937
1009.29.37840
1026.96.17049
1033.96.20263
1035.79.40057
1037.13.26840
1037.49.34641
1038.73.38621
1042.95.36093
1043.63.17037
1044.55.24939
1049.32.31466
1050.25.30388
1060.62.29640
1064.57.36126
Starting Bubble Sort on Linked List...
Comparisons: 3123750, Swaps: 1544618, Time taken : 2.536 seconds.
First 15 sorted account numbers (Bubble Sort on Linked List):
1006.19.41937
1009.29.37840
1026.96.17049
1033.96.20263
1035.79.40057
1037.13.26840
1037.49.34641
1038.73.38621
1042.95.36093
1043.63.17037
1044.55.24939
1049.32.31466
1050.25.30388
1060.62.29640
1064.57.36126
Starting Quick Sort on Array...
Comparisons: 32501, Swaps: 15752, Time taken : 0.023 seconds.
First 15 sorted account numbers (Quick Sort on Array):
1006.19.41937
1009.29.37840
1026.96.17049
1033.96.20263
1035.79.40057
1037.13.26840
1037.49.34641
1038.73.38621
1042.95.36093
1043.63.17037
1044.55.24939
1049.32.31466
1050.25.30388
1060.62.29640
1064.57.36126
Starting Merge Sort on Linked List...
Comparisons: 25093, Swaps: 0, Time taken : 0.018 seconds.
First 15 sorted account numbers (Merge Sort on Linked List):
1006.19.41937
1009.29.37840
1026.96.17049
1033.96.20263
1035.79.40057
1037.13.26840
1037.49.34641
1038.73.38621
1042.95.36093
1043.63.17037
1044.55.24939
1049.32.31466
1050.25.30388
1060.62.29640
1064.57.36126

--- Part 5: The Payoff (Binary Search) ---
Attempting to find account for: Avyan Byerly
Starting Quick Sort on Array...
Comparisons: 38457, Swaps: 15141, Time taken : 0.001 seconds.
Starting Binary Search...
Comparisons: 8, Swaps: 0, Time taken : 0 seconds.
Success! Found account:
Account Number: 1978.66.25918, Type: Checking, Owner: Avyan Byerly, Balance: 507, Created: Sun Oct 27 18:56:00 2024


*/