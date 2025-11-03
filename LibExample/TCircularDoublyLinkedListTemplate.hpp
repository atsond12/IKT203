// Implantation of the Circular Doubly Linked List

#ifndef TCIRCULARDOUBLYLINKEDLISTTEMPLATE_HPP
#define TCIRCULARDOUBLYLINKEDLISTTEMPLATE_HPP
#pragma once

#include "TDoublyLinkedListTemplate.hpp"

template <typename T>
class TCircularDoublyLinkedList : public TDoublyLinkedList<T> {
private:

    // Helper to remove a node, updating both next and prev pointers
    void InternalRemoveNode(TNode<T>* aNodeToDelete);

protected:
    // --- New Member Variable ---
    TNode<T>* cursor; // Points to the "current" node in the list.

public:
    using TSingleLinkedList<T>::Append;
    using TSingleLinkedList<T>::Prepend;

    // --- Constructor & Destructor ---
    TCircularDoublyLinkedList(bool aIsDataOwner);
    virtual ~TCircularDoublyLinkedList() override;

    // --- New Cursor Management Methods ---
    void ResetCursor();
    T GetCursorData() const;
    void AdvanceCursor(int aSteps = 1);
    void RewindCursor(int aSteps = 1);
    int GetCursorIndex() const;


    // --- Overridden Virtual Methods ---
    // For maintaining the circular link
    void Append(const T&) override;
    void Prepend(const T&) override;
    void Remove(const T&) override;
    void RemoveAll(const T&) override;
    T RemoveLast() override;
    void Reverse() override;
    void ReverseSublist(int, int) override;
    void Merge(TSingleLinkedList<T>& otherList) override;

    // For avoiding infinite loops
    bool Contains(const T&) const override;
    T Search(const T&, FCheckNode<T>) const override;
    void ForEach(FVisitNode<T>) const override;
    TNode<T>* GetMiddle() const override;
};

// Constructor: Establishes the initial circular link where the head points to itself.
template <typename T>
TCircularDoublyLinkedList<T>::TCircularDoublyLinkedList(bool aIsDataOwner)
    : TDoublyLinkedList<T>(aIsDataOwner) {
    // An empty circular list's dummy head points to itself.
    this->head->SetNext(this->head);
    this->head->SetPrev(this->head);
    this->cursor = this->head; // Cursor also starts at the head.
}

// Destructor: The parent destructors are virtual and will handle cleanup.
template <typename T>
TCircularDoublyLinkedList<T>::~TCircularDoublyLinkedList() {
    // Before the parent destructors run, we must break the circular link.
    // The parent's `nullptr`-terminated cleanup loop will now work correctly.
    if (!this->IsEmpty()) {
        // The tail is the node before the dummy head.
        // Set its 'next' pointer to null instead of back to the head.
        this->head->GetPrev()->SetNext(nullptr);
    }
    // Now, the chain is broken: head -> node1 -> ... -> tail -> nullptr
    // The TDoublyLinkedList and TSingleLinkedList destructors will be called
    // automatically after this, and they will now function correctly.
}

// Moves the cursor to the first element in the list.
template <typename T>
void TCircularDoublyLinkedList<T>::ResetCursor() {
    if (this->IsEmpty()) {
        this->cursor = this->head;
    }
    else {
        this->cursor = this->head->GetNext();
    }
}

// Returns the data at the cursor's current position.
template <typename T>
T TCircularDoublyLinkedList<T>::GetCursorData() const {
    // The cursor is only valid if it's not pointing to the dummy head.
    if (!this->IsEmpty() && this->cursor != this->head) {
        return this->cursor->GetData();
    }
    return nullptr;
}

// Moves the cursor forward, wrapping around if needed.
template <typename T>
void TCircularDoublyLinkedList<T>::AdvanceCursor(int aSteps) {
    if (this->IsEmpty()) {
        return;
    }

    for (int i = 0; i < aSteps; ++i) {
        this->cursor = this->cursor->GetNext();
        // If we move to the dummy head, we've wrapped around. Skip over it to the first element.
        if (this->cursor == this->head) {
            this->cursor = this->head->GetNext();
        }
    }
}

// Moves the cursor backward, wrapping around if needed.
template <typename T>
void TCircularDoublyLinkedList<T>::RewindCursor(int aSteps) {
    if (this->IsEmpty()) {
        return;
    }

    for (int i = 0; i < aSteps; ++i) {
        this->cursor = this->cursor->GetPrev();
        // If we move to the dummy head, we've wrapped around. Skip over it to the last element.
        if (this->cursor == this->head) {
            this->cursor = this->head->GetPrev();
        }
    }
}

// Gets the numerical index of the cursor's current position.
template <typename T>
int TCircularDoublyLinkedList<T>::GetCursorIndex() const {
    if (this->IsEmpty() || this->cursor == this->head) {
        return -1;
    }

    int index = 0;
    TNode<T>* current = this->head->GetNext();

    // Traverse the list safely, knowing it will eventually loop back to the head.
    while (current != this->head) {
        if (current == this->cursor) {
            return index;
        }
        current = current->GetNext();
        index++;
    }

    return -1; // Should not be reached if cursor is valid.
}

// Appends a node to the end of the list (before the dummy head).
// Use the InternAppend from TDoublyLinkedList for basic logic, then adjust prev pointers.
template <typename T>
void TCircularDoublyLinkedList<T>::Append(const T& aData) {
    // 1. Let the base class do ALL the standard work.
    TDoublyLinkedList<T>::Append(aData);
	// 2. Now fix the circular links.
    if(!this->IsEmpty()) {
        this->head->SetPrev(this->tail); // Dummy head's prev points to new tail
        this->tail->SetNext(this->head); // New tail's next points to dummy head
	}
}

// Prepends a node to the beginning of the list (after the dummy head).
// Use the Prepend from TDoublyLinkedList, this will handle all the logic.
template <typename T>
void TCircularDoublyLinkedList<T>::Prepend(const T& aData) {
	TDoublyLinkedList<T>::Prepend(aData);
	// No need to adjust circular links, as TDoublyLinkedList::Prepend already does it.
}


template <typename T>
void TCircularDoublyLinkedList<T>::InternalRemoveNode(TNode<T>* aNodeToDelete) {
    if (aNodeToDelete == nullptr || aNodeToDelete == this->head) return; // Invalid input

    // 1. Update cursor if it points to the node being deleted
    if (this->cursor == aNodeToDelete) {
        this->cursor = aNodeToDelete->GetNext();
    }

    //2. Update tail if the last node is being removed
    if (aNodeToDelete == this->tail) {
        this->tail = aNodeToDelete->GetPrev();
    }

    // 3. Relink neighbors (this maintains the circle automatically)
    TNode<T>* prevNode = aNodeToDelete->GetPrev();
    TNode<T>* nextNode = aNodeToDelete->GetNext();
    prevNode->SetNext(nextNode);
    nextNode->SetPrev(prevNode);

    // 4. Delete data if owned, then delete the node
    if (this->isDataOwner) {
        delete aNodeToDelete->GetData();
    }
    delete aNodeToDelete;
    this->size--;

    // 5. If the list is now empty, ensure tail and cursor point to the head.
    if (this->IsEmpty()) {
        this->tail = this->head;
        this->cursor = this->head;
    }
}

// Removes the first node with the given value.
template <typename T>
void TCircularDoublyLinkedList<T>::Remove(const T& aData) {
    if (this->IsEmpty()) return;

    // Start searching from the first actual node
    TNode<T>* current = this->head->GetNext();
    // Traverse the entire circle once
    for (int i = 0; i < this->size; ++i) {
        if (current->GetData() == aData) {
            TNode<T>* prevNode = current->GetPrev();
            TNode<T>* nextNode = current->GetNext();

            // Update tail if the last node is being removed
            if (current == this->tail) {
                this->tail = prevNode;
            }

            // Relink neighbors (this maintains the circle automatically)
            prevNode->SetNext(nextNode);
            nextNode->SetPrev(prevNode);

            if (this->isDataOwner) {
                delete current->GetData();
            }
            delete current;
            this->size--;
            return;
        }
        current = current->GetNext();
    }
}

// Removes the last element from the list.
template <typename T>
T TCircularDoublyLinkedList<T>::RemoveLast() {
    if (this->IsEmpty()) {
        return nullptr;
    }

    TNode<T>* nodeToRemove = this->tail;
    T dataToReturn = nodeToRemove->GetData();
    TNode<T>* newTail = nodeToRemove->GetPrev();

    // Relink the new tail to the head to maintain the circle
    newTail->SetNext(this->head);
    this->head->SetPrev(newTail);

    this->tail = newTail; // Update tail pointer

    delete nodeToRemove;
    this->size--;

    if (this->IsEmpty()) {
        this->tail = this->head; // Reset tail if list is now empty
    }

    return dataToReturn;
}

// Removes all occurrences of a given value using a safe, bounded loop.
template <typename T>
void TCircularDoublyLinkedList<T>::RemoveAll(const T& aData) {
    if (this->IsEmpty()) return;

    TNode<T>* current = this->head->GetNext();
    int initialSize = this->size; // Loop based on the original size

    for (int i = 0; i < initialSize; ++i) {
        TNode<T>* nextNode = current->GetNext(); // Get next node before potential deletion

        if (current->GetData() == aData) {
            TNode<T>* prevNode = current->GetPrev();

            if (current == this->tail) {
                this->tail = prevNode;
            }

            prevNode->SetNext(nextNode);
            nextNode->SetPrev(prevNode);

            if (this->isDataOwner) {
                delete current->GetData();
            }
            delete current;
            this->size--;
        }
        current = nextNode;
    }
    if (this->IsEmpty()) {
        this->tail = this->head;
    }
}

// Reverses the entire list by swapping the next/prev pointers of every node.
template <typename T>
void TCircularDoublyLinkedList<T>::Reverse() {
	if (this->size <= 1) return; // Nothing to reverse
    
	// 1. Keep track of original head and tail nodes.
    TNode<T>* newFirstNode = this->tail;

	// 2. Iterate through all nodes, swapping next and prev pointers.
    TNode<T>* current = this->head;
    // We must loop size + 1 times to include the dummy head in the pointer swap.
    for (int i = 0; i < this->size + 1; ++i) {
		current->SwapNextPrev();
		// The *new* prev pointer is the *original* next pointer, so this moves us forward.
		current = current->GetPrev();
    }
    
	// 3. Correct the head and tail pointers.
    this->tail = this->head->GetNext(); // The old head is the new tail.
	this->head->SetNext(newFirstNode); // The dummy head now points to the old tail.
	newFirstNode->SetPrev(this->head); // New first node's prev points to dummy head.
}

// Reverses a portion of the list, ensuring circular links are maintained.
template <typename T>
void TCircularDoublyLinkedList<T>::ReverseSublist(int start, int end) {
    if (start < 0 || end >= this->size || start >= end) {
        return;
    }

    // --- 1. Find boundary nodes ---
    TNode<T>* startNode = this->head->GetNext();
    for (int i = 0; i < start; ++i) {
        startNode = startNode->GetNext();
    }

    TNode<T>* endNode = startNode;
    for (int i = start; i < end; ++i) {
        endNode = endNode->GetNext();
    }

    TNode<T>* startPrev = startNode->GetPrev();
    TNode<T>* endNext = endNode->GetNext();

    // --- 2. Reverse pointers for all nodes within the sublist ---
    TNode<T>* current = startNode;
    for (int i = 0; i <= (end - start); ++i) {
        TNode<T>* temp = current->GetNext();
        current->SetNext(current->GetPrev());
        current->SetPrev(temp);
        current = temp;
    }

    // --- 3. Re-stitch the sublist (no nullptr checks needed) ---
    startPrev->SetNext(endNode);
    endNode->SetPrev(startPrev);
    startNode->SetNext(endNext);
    endNext->SetPrev(startNode);

    // Update tail pointer if it was part of the reversed segment
    if (this->head->GetPrev() == endNode) { // Original startNode is now at the end
        this->tail = startNode;
    }
}

// Merges another sorted list, creating a single, sorted circular list.
template <typename T>
void TCircularDoublyLinkedList<T>::Merge(TSingleLinkedList<T>& otherList) {
    // We can call the parent implementation to do the heavy lifting of merging.
    TDoublyLinkedList<T>::Merge(otherList);

    // The parent merge results in a null-terminated list. We just need to fix the ends.
    if (!this->IsEmpty()) {
        TNode<T>* firstNode = this->head->GetNext();
        this->tail->SetNext(firstNode);
        firstNode->SetPrev(this->tail);
    }
}

// Applies a function to each node using a safe loop for a circular list.
template <typename T>
void TCircularDoublyLinkedList<T>::ForEach(FVisitNode<T> aVisitNode) const {
    if (aVisitNode == nullptr || this->IsEmpty()) {
        return;
    }

    TNode<T>* current = this->head->GetNext();
    int index = 0;
    while (current != this->head) {
        aVisitNode(current->GetData(), index);
        current = current->GetNext();
        index++;
    }
}

// Checks if a value is in the list using a safe, non-infinite loop.
template <typename T>
bool TCircularDoublyLinkedList<T>::Contains(const T& aData) const {
    if (this->IsEmpty()) {
        return false;
    }

    TNode<T>* current = this->head->GetNext();
    while (current != this->head) {
        if (current->GetData() == aData) {
            return true;
        }
        current = current->GetNext();
    }
    return false;
}

// Searches for a value using a safe, non-infinite loop.
template <typename T>
T TCircularDoublyLinkedList<T>::Search(const T& aData, FCheckNode<T> aCheckNode) const {
    if (this->IsEmpty()) {
        return nullptr;
    }

    TNode<T>* current = this->head->GetNext();
    while (current != this->head) {
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
    return nullptr;
}

// Finds the middle node by traversing to the size/2 index.
template <typename T>
TNode<T>* TCircularDoublyLinkedList<T>::GetMiddle() const {
    if (this->IsEmpty()) return nullptr;

    // The safest way for a list where we know the size.
    int middleIndex = this->size / 2;
    TNode<T>* current = this->head->GetNext();
    for (int i = 0; i < middleIndex; ++i) {
        current = current->GetNext();
    }
    return current;
}

#endif // TCIRCULARDOUBLYLINKEDLISTTEMPLATE_HPP
