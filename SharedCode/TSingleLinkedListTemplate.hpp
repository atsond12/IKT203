#ifndef TSINGLELINKEDLIST_HPP
#define TSINGLELINKEDLIST_HPP

#pragma once

#include <type_traits> // Required for the Clone function, and prevent delete on non-pointer types
#include "TDoublyLinkedListTemplate.hpp"
#include "TCircularDoublyLinkedListTemplate.hpp"

template <typename T>
class TDoublyLinkedList; // Forward declaration for friendship
template <typename T>
class TCircularDoublyLinkedList; // Forward declaration for friendship

// Callback type definitions
template <typename T, typename TArgs>
using FDataFactory = T(*)(TArgs); // <-- ADD THIS LINE

template <typename T>
using FCheckNode = bool(*)(const T, const T);

template <typename T>
using FVisitNode = void(*)(const T, int);

template <typename T>
class TNode {
private:
	T data;        // Data of type T (e.g., TSong*)
	TNode* next;   // Pointer to the next node
	TNode* prev;   // Pointer to the previous node, prepared for future use

	void SwapNextPrev(); // Swaps the next and prev pointers of this node
public:
	// Constructor

	TNode(const T& aData);

	// Destructor
	~TNode();

	// Getters
	T GetData() const { return data; }
	TNode* GetNext() const { return next; }
	TNode* GetPrev() const { return prev; }

	// Setters
	void SetNext(TNode* aNextNode) { next = aNextNode; }
	void SetPrev(TNode* aPrevNode) { prev = aPrevNode; }

	// Add friendships here if needed
	friend class TDoublyLinkedList<T>;
	friend class TCircularDoublyLinkedList<T>;
};

// --- Method Implementations ---
// Constructor: Initializes the node with data from the factory function
template <typename T>
TNode<T>::TNode(const T& aData) : data(aData), next(nullptr), prev(nullptr) {}

// Destructor: Deletes the data pointer
template <typename T>
TNode<T>::~TNode() {
	//Do not delete data, data may exist outside the list
	data = nullptr; // Set data to nullptr to avoid dangling pointer
	next = nullptr; // Set next to nullptr to avoid dangling pointer
	prev = nullptr; // Set prev to nullptr to avoid dangling pointer
}

template <typename T>
void TNode<T>::SwapNextPrev() {
	TNode* temp = next;
	next = prev;
	prev = temp;
}

// --- End of TNode class ---

// Singly Linked List TSingleLinkedList using TNode<T> and dummy node
template <typename T>
class TSingleLinkedList {
private:
	// Helper to reset the list to empty state
	void ResetList();

protected:
	TNode<T>* head; // Pointer to the dummy head node
	TNode<T>* tail; // Pointer to the tail node
	int size;       // Current size of the list
	bool isDataOwner; // Indicates if the list owns the data

	// Internal implementations can be non-virtual
	TNode<T>* InternAppend(const T&);
	TNode<T>* InternPrepend(const T&);

	// Helper for RemoveAll to delete a node given its previous node
	void InternalRemoveNode(TNode<T>*, TNode<T>*);
	// Helper to get node at index (non-virtual, used internally)
	TNode<T>* GetNodeAtIndex(int aIndex) const;

public:
	// Constructor
	TSingleLinkedList(bool);
	// Virtual Destructor
	virtual ~TSingleLinkedList();

	// --- Core Operations (Virtual) ---
	virtual void Append(const T&);
	virtual void Prepend(const T&);
	virtual T GetAtIndex(int);
	virtual void Remove(const T&);
	virtual void Reverse();
	virtual void RemoveAll(const T&);
	virtual void ReverseSublist(int, int);
	virtual TNode<T>* GetMiddle() const;
	virtual void Merge(TSingleLinkedList<T>&);

	// --- Traversal functions that MUST now be virtual ---
	virtual bool Contains(const T&) const;
	virtual T Search(const T&, FCheckNode<T> = nullptr) const;
	virtual void ForEach(FVisitNode<T>) const;

	// --- Non-Virtual Methods ---
	template <typename TArgs>
	T Append(FDataFactory<T, TArgs>, TArgs);
	template <typename TArgs>
	T Prepend(FDataFactory<T, TArgs>, TArgs);
	int GetSize() const;
	bool IsEmpty() const;
	TSingleLinkedList<T> Clone() const;
	TNode<T>* StealNodes();
};

// Constructor: Initializes the dummy head node and list state
template <typename T>
TSingleLinkedList<T>::TSingleLinkedList(bool aIsDataOwner)
	: head(new TNode<T>(nullptr)), tail(head), size(0), isDataOwner(aIsDataOwner) {
	// COMPILE-TIME SAFETY CHECK
	// If the user tries to create a list of non-pointer types with ownership,
	// stop compilation with a static_assert.
	static_assert(std::is_pointer<T>::value || !aIsDataOwner,
		"isDataOwner can only be true if T is a pointer type (e.g., std::string*).");
}

// Virtual Destructor: Deletes all nodes and, if owner, the data
template <typename T>
TSingleLinkedList<T>::~TSingleLinkedList() {
	TNode<T>* current = head;
	while (current != nullptr) {
		TNode<T>* nextNode = current->GetNext();
		if (isDataOwner && current->GetData() != nullptr) {
			delete current->GetData(); // Delete data if the list owns it
		}
		delete current; // Free the node itself
		current = nextNode;
	}
}

template <typename T>
void TSingleLinkedList<T>::InternalRemoveNode(TNode<T>* aPrevNode, TNode<T>* aNodeToDelete) {
	if (aPrevNode == nullptr || aNodeToDelete == nullptr) return;
	aPrevNode->SetNext(aNodeToDelete->GetNext());
	if (aNodeToDelete == this->tail) {
		this->tail = aPrevNode;
	}
	if (this->isDataOwner) {
		delete aNodeToDelete->GetData();
	}
	delete aNodeToDelete;
	this->size--;
}


// StealNodes: Detaches and returns the list's nodes, leaving it empty
// Uses ResetList to clear the list state
template <typename T>
TNode<T>* TSingleLinkedList<T>::StealNodes() {
	TNode<T>* stolenHead = head->GetNext(); // First actual node
	this->ResetList(); // Clear the list to empty state
	return stolenHead; // Return the detached nodes
}

// InternAppend: Handles the logic of adding a node to the end
template <typename T>
TNode<T>* TSingleLinkedList<T>::InternAppend(const T& aData) {
	TNode<T>* newNode = new TNode<T>(aData);
	tail->SetNext(newNode); // Link the old tail to the new node
	tail = newNode;         // Update the tail pointer
	size++;
	return newNode;
}

// Public Append method
template <typename T>
void TSingleLinkedList<T>::Append(const T& aData) {
	InternAppend(aData);
}

// InternPrepend: Handles the logic of adding a node to the beginning
template <typename T>
TNode<T>* TSingleLinkedList<T>::InternPrepend(const T& aData) {
	TNode<T>* newNode = new TNode<T>(aData);
	newNode->SetNext(head->GetNext());
	head->SetNext(newNode);
	if (tail == head) { // If the list was empty, new node is also the tail
		tail = newNode;
	}
	size++;
	return newNode;
}

// Public Prepend method
template <typename T>
void TSingleLinkedList<T>::Prepend(const T& aData) {
	InternPrepend(aData);
}

// Append(): Creates a new node with the given factory and returns the new node's data.
template <typename T>
template <typename TArgs>
T TSingleLinkedList<T>::Append(FDataFactory<T, TArgs> aDataFactory, TArgs aArgs) {
	if (aDataFactory == nullptr) {
		return nullptr; // Return nullptr if no factory is provided
	}
	T newData = aDataFactory(aArgs);
	this->Append(newData); // Call the existing virtual Append(T) method
	return newData;
}

// Prepend(): Creates a new node with the given factory and returns the new node's data.
template <typename T>
template <typename TArgs>
T TSingleLinkedList<T>::Prepend(FDataFactory<T, TArgs> aDataFactory, TArgs aArgs) {
	if (aDataFactory == nullptr) {
		return nullptr; // Return nullptr if no factory is provided
	}
	T newData = aDataFactory(aArgs);
	this->Prepend(newData); // Call the existing virtual Prepend(T) method
	return newData;
}


template <typename T>
TNode<T>* TSingleLinkedList<T>::GetNodeAtIndex(int aIndex) const {
	TNode<T>* current = head->GetNext(); // Start at the first actual node
	for (int i = 0; i < aIndex; ++i) {
		current = current->GetNext();
	}
	return current;
}

// GetAtIndex: Returns the value at a specified index.
// This version iterates from the beginning only, as it cannot go backward.
template <typename T>
T TSingleLinkedList<T>::GetAtIndex(int aIndex) {
	if (aIndex < 0 || aIndex >= size) {
		return nullptr; // Index out of bounds
	}
	return GetNodeAtIndex(aIndex)->GetData();
}

// Remove: Removes the first node matching the given value.
// Requires tracking the previous node to relink the list.
template <typename T>
void TSingleLinkedList<T>::Remove(const T& aData) {
	TNode<T>* prev = head;
	TNode<T>* current = head->GetNext();

	while (current != nullptr) {
		if (current->GetData() == aData) {
			// Match found, remove the node
			InternalRemoveNode(prev, current);
			return; // Only remove the first occurrence
		}
	}
	prev = current;
	current = current->GetNext();
}

// Reverse: Reverses the list using the classic iterative algorithm for singly-linked lists.
template <typename T>
void TSingleLinkedList<T>::Reverse() {
	if (size <= 1) {
		return; // Nothing to reverse
	}

	// The original first node will become the new tail
	tail = head->GetNext();

	TNode<T>* prevNode = nullptr;
	TNode<T>* currentNode = head->GetNext();
	TNode<T>* nextNode = nullptr;

	while (currentNode != nullptr) {
		nextNode = currentNode->GetNext();   // Store next node
		currentNode->SetNext(prevNode);      // Reverse the current node's pointer
		prevNode = currentNode;              // Move pointers one position ahead
		currentNode = nextNode;
	}

	// After the loop, prevNode is the new first node
	head->SetNext(prevNode);
}

// Contains: Checks if the list contains the given value,
// this is diffrent from Search as it only checks for existence
// This using search with a nullptr as the check function
template <typename T>
bool TSingleLinkedList<T>::Contains(const T& aData) const {
	return Search(aData, nullptr) != nullptr;
}

// Search: Finds a value using an optional custom comparison function
template <typename T>
T TSingleLinkedList<T>::Search(const T& aData, FCheckNode<T> aCheckNode) const {
	TNode<T>* current = head->GetNext();
	while (current != nullptr) {
		// Use the provided check function or default to direct comparison
		if (aCheckNode != nullptr) {
			if (aCheckNode(current->GetData(), aData)) {
				return current->GetData();
			}
		}
		else {
			if (current->GetData() == aData) {
				return current->GetData();
			}
		}
		current = current->GetNext();
	}
	return nullptr; // Not found
}

// ForEach: Applies a function to each node in the list
template <typename T>
void TSingleLinkedList<T>::ForEach(FVisitNode<T> aVisitNode) const {
	if (aVisitNode == nullptr) {
		return;
	}
	TNode<T>* current = head->GetNext();
	int index = 0;
	while (current != nullptr) {
		aVisitNode(current->GetData(), index);
		current = current->GetNext();
		index++;
	}
}

// GetSize: Returns the current number of elements in the list
template <typename T>
int TSingleLinkedList<T>::GetSize() const {
	return size;
}

// IsEmpty: Checks if the list has any elements
template <typename T>
bool TSingleLinkedList<T>::IsEmpty() const {
	return size == 0;
}

// RemoveAll: Removes all occurrences of a given value from the list.
template <typename T>
void TSingleLinkedList<T>::RemoveAll(const T& aData) {
	TNode<T>* prev = this->head;
	TNode<T>* current = this->head->GetNext();

	while (current != nullptr) {
		if (current->GetData() == aData) {
			// Match found, remove the node
			TNode<T>* nodeToDelete = current;
			current = current->GetNext(); // Advance current before deletion
			InternalRemoveNode(prev, nodeToDelete);
			// prev remains the same, as we just removed current
		}
		else {
			// No match, advance both pointers
			prev = current;
			current = current->GetNext();
		}
	}
}

// Clone: Creates a deep copy of the list.
template <typename T>
TSingleLinkedList<T> TSingleLinkedList<T>::Clone() const {
	// Create a new list with the same ownership policy.
	TSingleLinkedList<T> newList(this->isDataOwner);

	TNode<T>* current = this->head->GetNext();
	while (current != nullptr) {
		T dataToCopy = current->GetData();

		// This is the core of the deep copy logic.
		if (this->isDataOwner && std::is_pointer<T>::value && dataToCopy != nullptr) {
			// If the list owns its pointer data, we must create a NEW object.
			// This assumes the underlying type has a copy constructor.
			// `std::remove_pointer_t<T>` gets the type T points to (e.g., TSong from TSong*).
			newList.Append(new std::remove_pointer_t<T>(*dataToCopy));
		}
		else {
			// For value types (int, double) or non-owned pointers, just copy the value.
			newList.Append(dataToCopy);
		}
		current = current->GetNext();
	}
	return newList;
}

// ReverseSublist: Reverses a portion of the list between two indices (inclusive).
template <typename T>
void TSingleLinkedList<T>::ReverseSublist(int aStart, int aEnd) {
	// Validate indices
	if (aStart < 0 || aEnd >= this->size || aStart >= aEnd) return;


	// --- 1. Traverse to the nodes that define the sublist boundaries ---
	// Use the helper to find the node *before* the sublist starts
	TNode<T>* startNodePrev = (aStart == 0) ? this->head : this->GetNodeAtIndex(aStart - 1); // Node before the start of the sublist
	TNode<T>* sublistHead = startNodePrev->GetNext(); // First node of the sublist

	// --- 2. Perform standard reversal on the sublist part only ---
	TNode<T>* prevNode = nullptr;
	TNode<T>* currentNode = sublistHead;
	TNode<T>* nextNode = nullptr;
	for (int i = 0; i <= (aEnd - aStart); ++i) {
		nextNode = currentNode->GetNext();
		currentNode->SetNext(prevNode);
		prevNode = currentNode;
		currentNode = nextNode;
	}

	// --- 3. Stitch the reversed sublist back into the main list ---
	// 'prevNode' is now the new head of the reversed sublist.
	// 'sublistHead' is now the tail of the reversed sublist.
	startNodePrev->SetNext(prevNode);
	sublistHead->SetNext(currentNode);

	// Update the main tail pointer if the reversal included the original tail.
	if (aEnd == this->size - 1) {
		this->tail = sublistHead;
	}
}

// GetMiddle: Finds the middle node of the list using the fast/slow pointer algorithm.
template <typename T>
TNode<T>* TSingleLinkedList<T>::GetMiddle() const {
	if (this->IsEmpty()) {
		return nullptr;
	}

	TNode<T>* slow = this->head->GetNext();
	TNode<T>* fast = this->head->GetNext();

	// The loop condition ensures 'fast' and 'fast->GetNext()' are valid.
	// When 'fast' reaches the end, 'slow' will be at the midpoint.
	while (fast != nullptr && fast->GetNext() != nullptr) {
		slow = slow->GetNext();
		fast = fast->GetNext()->GetNext();
	}

	return slow;
}

template <typename T>
void TSingleLinkedList<T>::ResetList() {
	// Clear the list to an empty state
	this->head->SetNext(nullptr);
	this->tail = this->head;
	this->size = 0;
}

template <typename T>
void TSingleLinkedList<T>::Merge(TSingleLinkedList<T>& aOtherList) {
	// If the other list is empty, there's nothing to do.
	if (aOtherList.IsEmpty()) {
		return;
	}

	// If this list is empty, take ownership of the other list's nodes.
	if (this->IsEmpty()) {
		this->head->SetNext(aOtherList.head->GetNext());
		this->tail = aOtherList.tail;
		this->size = aOtherList.size;

		// Clear the other list
		aOtherList.head->SetNext(nullptr);
		aOtherList.tail = aOtherList.head;
		aOtherList.size = 0;
		return;
	}

	// Pointers to the current nodes in each list
	TNode<T>* p1 = this->head->GetNext();
	TNode<T>* p2 = aOtherList.head->GetNext();

	// Use the dummy head of `this` list to start building the merged result.
	TNode<T>* tail = this->head;

	// --- Main Loop: Traverse both lists and pick the smaller node ---
	while (p1 != nullptr && p2 != nullptr) {
		if (p1->GetData() <= p2->GetData()) {
			tail->SetNext(p1);
			p1 = p1->GetNext();
		}
		else {
			tail->SetNext(p2);
			p2 = p2->GetNext();
		}
		tail = tail->GetNext();
	}

	// --- Append the Remainder ---
	tail->SetNext(p1 != nullptr ? p1 : p2);

	// --- Update the Tail Pointer ---
	while (tail->GetNext() != nullptr) {
		tail = tail->GetNext();
	}
	this->tail = tail;

	// --- Update Size ---
	this->size += aOtherList.size;

	// --- Clear the Other List ---
	aOtherList.ResetList();
}

// --- End of TSingleLinkedList class ---

#endif // !TSINGLELINKEDLIST_HPP