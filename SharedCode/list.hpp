#pragma once
#include <stdexcept> // For std::runtime_error
#include <string>    // For std::to_string

// type defined function for data factory
template <typename T>
using FDataFactory = T(*)();

// Linked List using TNode<T> class with constructor and destructor
template <typename T>
class TNode {
private:
	T* data;        // Data of type T
	TNode* next;    // Pointer to the next node
	TNode* prev;    // Pointer to the previous node (new addition)
public:
	// Constructor
	TNode(FDataFactory<T> aDataFactory = nullptr);

	// Destructor
	~TNode();

	// Getters
	T* GetData() const { return data; }
	TNode* GetNext() const { return next; }
	TNode* GetPrev() const { return prev; }

	// Setters
	void SetNext(TNode* aNextNode) { next = aNextNode; }
	void SetPrev(TNode* aPrevNode) { prev = aPrevNode; }
};
// --- Method Implementations ---
// Constructor: Initializes the node with data from the factory function
template <typename T>
TNode<T>::TNode(FDataFactory<T> aDataFactory) : next(nullptr), prev(nullptr) {
	if (aDataFactory != nullptr) {
		// Call the factory function to get a T object, then create a new T object on the heap
		data = new T(aDataFactory());
	}
	else {
		// Default initialization if no factory is provided
		data = new T();
	}
}

// Destructor: Deletes the data pointer
template <typename T>
TNode<T>::~TNode() {
	delete data; // Free the memory allocated for data
	data = nullptr; // Set data to nullptr to avoid dangling pointer	
	next = nullptr; // Set next to nullptr to avoid dangling pointer
	prev = nullptr; // Set prev to nullptr to avoid dangling pointer
}
// --- End of TNode class ---



//Type defined functions for TLinkedList
// FCheckNode, a function pointer type for checking nodes, use this to define custom checks
// aValue is the value to check, returns true if the value matches the criteria
// No need to provide the original data to check against, the caller has this information
template <typename T>
using FCheckNode = bool(*)(const T*, const T*);

template <typename T>
using FVisitNode = void(*)(const T* aData, int aIndex);

// Linked List TLinkedList using TNode<T> and dummy node
template <typename T>
class TLinkedList {
private:
	TNode<T>* head; // Pointer to the dummy head node
	TNode<T>* tail; // Pointer to the tail node
	int size;       // Current size of the list
public:
	// Constructor
	TLinkedList();
	// Destructor
	~TLinkedList();

	// Core Linked List Operations
	void Append(FDataFactory<T> aDataFactory);
	void Prepend(FDataFactory<T> aDataFactory);
	T* GetAtIndex(int aIndex);
	void Remove(const T* aValue);

	// Finding and Checking Operations
	bool Contains(const T* aValue) const;
	T* Search(const T* aValue, FCheckNode<T> checkNode = nullptr) const;

	// Loop Operations
	void ForEach(FVisitNode<T> aVisitNode) const;

	// Helper Functions
	int GetSize() const;
	bool IsEmpty() const;
	void Reverse() const;
};

// --- Method Implementations ---
// Constructor: Initializes the dummy head node and tail
template <typename T>
TLinkedList<T>::TLinkedList() : size(0) {
	head = new TNode<T>(nullptr); // Create a dummy head node
	tail = head;              // Initially, tail is the same as head
}

// Destructor: Deletes all nodes in the list
template <typename T>
TLinkedList<T>::~TLinkedList() {
	TNode<T>* current = head;
	while (current != nullptr) {
		TNode<T>* nextNode = current->GetNext();
		delete current; // Free the current node
		current = nextNode; // Move to the next node
	}
}

// Add(): Adds a new node with the given aValue to the end of the list
template <typename T>
void TLinkedList<T>::Append(FDataFactory<T> aDataFactory) {
	TNode<T>* newNode = new TNode<T>(aDataFactory);
	newNode->SetPrev(tail); // Set the prev pointer of the new node
	tail->SetNext(newNode); // Update the next pointer of the current tail
	tail = newNode;         // Update the tail to the new node
	size++;
}

// Prepend(): Adds a new node with the given aValue to the beginning of the list
template <typename T>
void TLinkedList<T>::Prepend(FDataFactory<T> aDataFactory) {
	TNode<T>* newNode = new TNode<T>(aDataFactory);
	newNode->SetNext(head->GetNext());
	if (head->GetNext() != nullptr) {
		head->GetNext()->SetPrev(newNode); // Update the prev pointer of the first node
	}
	head->SetNext(newNode);
	newNode->SetPrev(head); // Set the prev pointer of the new node
	if (tail == head) {
		tail = newNode;
	}
	size++;
}

// GetAtIndex(): Returns the value at the specified index
template <typename T>
T* TLinkedList<T>::GetAtIndex(int aIndex) {
	if (aIndex < 0 || aIndex >= size) {
		return nullptr; // Return nullptr if index is out of bounds
	}
	TNode<T>* current;
	// If the index is in the first half, traverse from head, otherwise traverse from tail
	if (aIndex < size / 2) {
		current = head->GetNext();
		for (int i = 0; i < aIndex; i++) {
			current = current->GetNext();
		}
	}
	else {
		current = tail;
		for (int i = size - 1; i > aIndex; i--) {
			current = current->GetPrev();
		}
	}
	return current->GetData();
}

template <typename T>
void TLinkedList<T>::Remove(const T* aValue) {
    TNode<T>* current = head;
    while (current->GetNext() != nullptr) {
        if (current->GetNext()->GetData() == aValue) {
            TNode<T>* nodeToDelete = current->GetNext();
            current->SetNext(nodeToDelete->GetNext());
            if (nodeToDelete->GetNext() != nullptr) {
                nodeToDelete->GetNext()->SetPrev(current); // Update the prev pointer
            }
            if (nodeToDelete == tail) {
                tail = current;
            }
            delete nodeToDelete;
            size--;
            return;
        }
        current = current->GetNext();
    }
}

template <typename T>
bool TLinkedList<T>::Contains(const T* aValue) const {
    TNode<T>* current = head->GetNext();
    while (current != nullptr) {
        if (current->GetData() == aValue) {
            return true;
        }
        current = current->GetNext();
    }
    return false;
}

// Search(): Searches for a node with the given value using an optional check function
template <typename T>
T* TLinkedList<T>::Search(const T* aValue, FCheckNode<T> aCheckNode) const {
	if (aCheckNode == nullptr) {
		return nullptr; // Return nullptr if no check function is provided
	}
	TNode<T>* current = head->GetNext();
	while (current != nullptr) {
		const T* currentData = current->GetData();
		const bool isMatch = aCheckNode(currentData, aValue);
		if (isMatch) return (T*)currentData; // Return the data if it matches the criteria
		current = current->GetNext();
	}
	return nullptr; // Return nullptr if no match is found
}


// GetSize(): Returns the current size of the list
template <typename T>
int TLinkedList<T>::GetSize() const {
	return size;
}
// IsEmpty(): Checks if the list is empty
template <typename T>
bool TLinkedList<T>::IsEmpty() const {
	return size == 0;
}

template <typename T>
void TLinkedList<T>::Reverse() const {
	TNode<T>* current = head->GetNext();
	TNode<T>* prev = nullptr;
	while (current != nullptr) {
		TNode<T>* nextNode = current->GetNext();
		current->SetNext(prev); // Reverse the next pointer
		current->SetPrev(nextNode); // Set the prev pointer to the next node
		prev = current; // Move prev to the current node
		current = nextNode; // Move to the next node
	}
	((TLinkedList<T>*)this)->tail = head->GetNext(); // Update tail to the new first node
	head->SetNext(prev); // Set head's next to the new first node
	if (prev != nullptr) {
		prev->SetPrev(head); // Set the new first node's prev to head
	}
}

// ForEach(): Applies a function to each node in the list
template <typename T>
void TLinkedList<T>::ForEach(FVisitNode<T> aVisitNode) const {
	if (aVisitNode == nullptr) {
		return; // Return if no visit function is provided
	}
	TNode<T>* current = head->GetNext();
	int index = 0;
	while (current != nullptr) {
		aVisitNode(current->GetData(), index); // Call the visit function with the data and index
		current = current->GetNext();
		index++;
	}
}

// --- End of TLinkedList class ---
