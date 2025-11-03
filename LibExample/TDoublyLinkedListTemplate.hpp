// TDoublyLinkedList implements a doubly linked list data structure with basic functionalities.

#ifndef TDOUBLYLINKEDLISTTEMPLEATE_HPP
#define TDOUBLYLINKEDLISTTEMPLEATE_HPP
#pragma once
#include "TSingleLinkedListTemplate.hpp"

// Doubly Linked List inheriting from TSingleLinkedList
template <typename T>
class TDoublyLinkedList : public TSingleLinkedList<T> {
private:
	// Internal helper to remove a node, updating both next and prev pointers
	void InternalRemoveNode(TNode<T>* aNodeToDelete);

public:
	using TSingleLinkedList<T>::Append;
	using TSingleLinkedList<T>::Prepend;

	// --- Constructor & Destructor ---
	// Constructor calls the base class's constructor to initialize common members.
	TDoublyLinkedList(bool aIsDataOwner);

	// The destructor is also virtual, ensuring the correct cleanup chain.
	virtual ~TDoublyLinkedList() override;


	// --- Overridden Virtual Methods from TSingleLinkedList ---
	// These methods provide new implementations to manage the 'prev' pointer
	// and/or to improve performance.
	virtual void Append(const T&) override;
	virtual void Prepend(const T&) override;
	virtual T GetAtIndex(int) override;
	virtual void Remove(const T&) override;
	virtual void Reverse() override;
	virtual void Merge(TSingleLinkedList<T>&) override;
	virtual void RemoveAll(const T&) override;
	virtual void ReverseSublist(int, int) override;
	virtual T RemoveLast();


	// --- New Methods Unique to TDoublyLinkedList ---
	// These methods leverage the 'prev' pointer for capabilities that are
	// inefficient or impossible in a singly-linked list.

	void InsertBefore(TNode<T>*, T&);
	void ForEachReverse(FVisitNode<T>) const;
};


template<typename T>
inline void TDoublyLinkedList<T>::InternalRemoveNode(TNode<T>* aNodeToDelete)
{
	if (aNodeToDelete == nullptr || aNodeToDelete == this->head) return;
	TNode<T>* prevNode = aNodeToDelete->GetPrev();
	TNode<T>* nextNode = aNodeToDelete->GetNext();
	if (prevNode != nullptr) {
		prevNode->SetNext(nextNode);
	}
	if (nextNode != nullptr) {
		nextNode->SetPrev(prevNode);
	}
	if (aNodeToDelete == this->tail) {
		this->tail = prevNode;
	}
	if (this->isDataOwner) {
		delete aNodeToDelete->GetData();
	}
	delete aNodeToDelete;
	this->size--;
}

// Constructor: Calls the base class constructor to perform initialization.
template <typename T>
TDoublyLinkedList<T>::TDoublyLinkedList(bool aIsDataOwner)
	: TSingleLinkedList<T>(aIsDataOwner) {}

// Destructor: The base class destructor is virtual, so this is called automatically.
// No new resources were allocated, so the body can be empty.
template <typename T>
TDoublyLinkedList<T>::~TDoublyLinkedList() {} // No additional cleanup needed, the base class handles it.

// Append: Overridden to correctly set the 'prev' pointer on the new node.
template <typename T>
void TDoublyLinkedList<T>::Append(const T& aData) {
	TNode<T>* oldTail = this->tail;
	TNode<T>* newNode = this->InternAppend(aData);
	newNode->SetPrev(oldTail); // Set the new node's prev to the old tail
}

// Prepend: Overridden to correctly set 'prev' pointers on the new node and its old neighbor.
template <typename T>
void TDoublyLinkedList<T>::Prepend(const T& aData) {
	TNode<T>* newNode = this->InternPrepend(aData);
	newNode->SetPrev(this->head); // New node's prev points to dummy head
	if (newNode->GetNext() != nullptr) {
		newNode->GetNext()->SetPrev(newNode); // Old first node's prev points to new node
	}
}

// GetAtIndex: Re-introduces the performance optimization to search from the tail.
template <typename T>
T TDoublyLinkedList<T>::GetAtIndex(int aIndex) {
	if (aIndex < 0 || aIndex >= this->size) {
		return nullptr; // Index out of bounds
	}

	TNode<T>* current;
	// Check which end is closer and traverse from there.
	if (aIndex < this->size / 2) {
		current = this->head->GetNext();
		for (int i = 0; i < aIndex; i++) {
			current = current->GetNext();
		}
	}
	else {
		current = this->tail;
		for (int i = this->size - 1; i > aIndex; i--) {
			current = current->GetPrev();
		}
	}
	return current->GetData();
}

// Remove: Removes all nodes matching the given value.
template <typename T>
void TDoublyLinkedList<T>::Remove(const T& aData) {
	TNode<T>* current = this->head->GetNext();
	while (current != nullptr) {
		if (current->GetData() == aData) {
			TNode<T>* nodeToRemove = current;
			current = current->GetNext(); // Move current forward before deletion
			InternalRemoveNode(nodeToRemove);
		}
		else {
			// No match, just advance current
			current = current->GetNext();
		}
	}
}

// Reverse: Uses the much simpler algorithm of swapping prev/next pointers.
template <typename T>
void TDoublyLinkedList<T>::Reverse() {
	if (this->size <= 1) return;

	// 1. Keep track of original head and tail nodes.
	TNode<T>* originalHeadNode = this->head->GetNext();
	TNode<T>* originalTailNode = this->tail;

	// 2. Swap prev/next for every node.
	TNode<T>* current = originalHeadNode;
	while (current != nullptr) {
		current->SwapNextPrev();
		// The *new* prev pointer is the *original* next pointer, so this moves us forward.
		current = current->GetPrev();
	}

	// 3. Correct the pointers for the new head and tail.
	this->tail = originalHeadNode; // The old head is the new tail.
	this->head->SetNext(originalTailNode); // The dummy head now points to the old tail.
}

// RemoveLast: Removes the final element in O(1) time.
// Note: This returns the data, so the caller is now responsible for it.
// The data itself is not deleted, even if isDataOwner is true.
template <typename T>
T TDoublyLinkedList<T>::RemoveLast() {
	if (this->IsEmpty()) {
		return nullptr;
	}

	TNode<T>* nodeToRemove = this->tail;
	T dataToReturn = nodeToRemove->GetData();

	this->tail = this->tail->GetPrev();
	this->tail->SetNext(nullptr);

	delete nodeToRemove;
	this->size--;

	return dataToReturn;
}

// InsertBefore: Inserts a new node before a specified node in O(1) time.
template <typename T>
void TDoublyLinkedList<T>::InsertBefore(TNode<T>* aNode, T& aData) {
	if (aNode == nullptr || aNode == this->head) {
		// Cannot insert before a null node or the dummy head
		return;
	}

	// If inserting before the first element, Prepend is easier.
	if (aNode == this->head->GetNext()) {
		Prepend(aData);
		return;
	}

	TNode<T>* newNode = new TNode<T>(aData);
	TNode<T>* prevNode = aNode->GetPrev();

	// Link new node to its neighbors
	newNode->SetPrev(prevNode);
	newNode->SetNext(aNode);

	// Update neighbors to point to the new node
	prevNode->SetNext(newNode);
	aNode->SetPrev(newNode);

	this->size++;
}

// ForEachReverse: Traverses the list backward from tail to head.
template <typename T>
void TDoublyLinkedList<T>::ForEachReverse(FVisitNode<T> aVisitNode) const {
	if (aVisitNode == nullptr) {
		return;
	}
	TNode<T>* current = this->tail;
	int index = this->size - 1;
	while (current != this->head) {
		aVisitNode(current->GetData(), index);
		current = current->GetPrev();
		index--;
	}
}

template <typename T>
void TDoublyLinkedList<T>::Merge(TSingleLinkedList<T>& otherList) {
	if (otherList.IsEmpty()) {
		return;
	}

	int otherSize = otherList.GetSize();
	// Call the now-public StealNodes to legally take ownership of the nodes.
	TNode<T>* p2 = otherList.StealNodes();

	if (this->IsEmpty()) {
		this->head->SetNext(p2);
		if (p2) p2->SetPrev(this->head);
		this->size = otherSize;
		// Find the new tail by traversing.
		TNode<T>* newTail = this->head;
		while (newTail->GetNext() != nullptr) {
			newTail = newTail->GetNext();
		}
		this->tail = newTail;
		return;
	}

	TNode<T>* p1 = this->head->GetNext();
	TNode<T>* currentTail = this->head;

	while (p1 != nullptr && p2 != nullptr) {
		if (p1->GetData() <= p2->GetData()) {
			currentTail->SetNext(p1);
			p1->SetPrev(currentTail);
			p1 = p1->GetNext();
		}
		else {
			currentTail->SetNext(p2);
			p2->SetPrev(currentTail);
			p2 = p2->GetNext();
		}
		currentTail = currentTail->GetNext();
	}

	TNode<T>* remainder = (p1 != nullptr) ? p1 : p2;
	currentTail->SetNext(remainder);
	if (remainder != nullptr) {
		remainder->SetPrev(currentTail);
	}

	this->size += otherSize;
	// After merging, find the new tail by traversing from where we left off.
	while (currentTail->GetNext() != nullptr) {
		currentTail = currentTail->GetNext();
	}
	this->tail = currentTail;
}

// Remove: Removes all nodes matching the given value.
template <typename T>
void TDoublyLinkedList<T>::RemoveAll(const T& aValue) {
	TNode<T>* current = this->head->GetNext();
	while (current != nullptr) {
		if (current->GetData() == aValue) {
			TNode<T>* nodeToRemove = current;
			current = current->GetNext(); // Move current forward before deletion
			InternalRemoveNode(nodeToRemove);
		}
		else {
			// No match, just advance the pointer
			current = current->GetNext();
		}
	}
}

// ReverseSublist: Reverses a portion of the list between two indices (inclusive).
template <typename T>
void TDoublyLinkedList<T>::ReverseSublist(int start, int end) {
	if (start < 0 || end >= this->size || start >= end) return;
	
	// --- 1. Find boundary nodes ---
	TNode<T>* startNode = this->head->GetNext();
	TNode<T>* endNode = startNode;

	for (int i = 0; i < start; ++i) {
		startNode = startNode->GetNext();
	}
	for (int i = start; i < end; ++i) {
		endNode = endNode->GetNext();
	}

	TNode<T>* startPrev = startNode->GetPrev();
	TNode<T>* endNext = endNode->GetNext();

	// --- 2. Reverse pointers for all nodes within the sublist ---
	TNode<T>* current = startNode;
	while (current != endNext) {
		TNode<T>* temp = current->GetNext();
		current->SetNext(current->GetPrev());
		current->SetPrev(temp);
		current = temp; // Move to the original next node
	}

	// --- 3. Re-stitch the reversed sublist into the main list ---
	startPrev->SetNext(endNode);
	endNode->SetPrev(startPrev);

	startNode->SetNext(endNext);
	if (endNext != nullptr) {
		endNext->SetPrev(startNode);
	}
	else {
		// If we reversed to the end, the original startNode is the new tail
		this->tail = startNode;
	}
}

#endif // !TDOUBLYLINKEDLISTTEMPLEATE_HPP
