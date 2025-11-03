#pragma once
#include <stdexcept> // For std::runtime_error
#include <string>    // For std::to_string

// Linked List using TNode<T> class with constructor and destructor
template <typename T>
class TNode {
private:
    T data;        // Data of type T (e.g., TSong*)
    TNode* next;   // Pointer to the next node
    TNode* prev;   // Pointer to the previous node
public:
    // Constructor

    TNode(T aData);

    // Destructor
    ~TNode();

    // Getters
    T GetData() const { return data; }
    TNode* GetNext() const { return next; }
    TNode* GetPrev() const { return prev; }

    // Setters
    void SetNext(TNode* aNextNode) { next = aNextNode; }
    void SetPrev(TNode* aPrevNode) { prev = aPrevNode; }
};

// --- Method Implementations ---
// Constructor: Initializes the node with data from the factory function
template <typename T>
TNode<T>::TNode(T aData) : data(aData), next(nullptr), prev(nullptr) {}

// Destructor: Deletes the data pointer
template <typename T>
TNode<T>::~TNode() {
	//Do not delete data, data may exist outside the list
    data = nullptr; // Set data to nullptr to avoid dangling pointer
    next = nullptr; // Set next to nullptr to avoid dangling pointer
    prev = nullptr; // Set prev to nullptr to avoid dangling pointer
}

// --- End of TNode class ---

// Type defined function for data factory
template <typename T, typename TArgs>
using FDataFactory = T(*)(TArgs);

// Type defined functions for TLinkedList
template <typename T>
using FCheckNode = bool(*)(const T, const T);

template <typename T>
using FVisitNode = void(*)(const T, int);

// Linked List TLinkedList using TNode<T> and dummy node
template <typename T>
class TLinkedList {
private:
    TNode<T>* head; // Pointer to the dummy head node
    TNode<T>* tail; // Pointer to the tail node
    int size;       // Current size of the list
	bool isDataOwner; // Indicates if the list owns the data and should delete it

    void InternAppend(T);
    void InternPrepend(T);

public:
    // Constructor
    TLinkedList(bool);
    // Destructor
    ~TLinkedList();

    // Core Linked List Operations
    template <typename TArgs>
    T Append(FDataFactory<T, TArgs>, TArgs);
	void Append(T);

    template <typename TArgs>
    T Prepend(FDataFactory<T, TArgs>, TArgs);
	void Prepend(T aValue);

    T GetAtIndex(int aIndex);
    void Remove(const T aValue);

    // Finding and Checking Operations
    bool Contains(const T aValue) const;
    T Search(const T aValue, FCheckNode<T> checkNode = nullptr) const;

    // Loop Operations
    void ForEach(FVisitNode<T> aVisitNode) const;

    // Helper Functions
    int GetSize() const;
    bool IsEmpty() const;
    void Reverse();
};

// --- Method Implementations ---
// Constructor: Initializes the dummy head node and tail
template <typename T>
TLinkedList<T>::TLinkedList(bool aIsDataOwner) : size(0) {
	isDataOwner = aIsDataOwner;
    head = new TNode<T>(nullptr); // Create a dummy head node
    tail = head;                 // Initially, tail is the same as head
}

// Destructor: Deletes all nodes in the list
template <typename T>
TLinkedList<T>::~TLinkedList() {
    TNode<T>* current = head;
    while (current != nullptr) {
        TNode<T>* nextNode = current->GetNext();
		if (isDataOwner && current->GetData() != nullptr) {
			delete current->GetData(); // Delete the data if the list owns it
		}
        delete current; // Free the current node
        current = nextNode; // Move to the next node
    }
}

template <typename T>
void TLinkedList<T>::InternAppend(T aData) {
    TNode<T>* newNode = new TNode<T>(aData);
    newNode->SetPrev(tail); // Set the prev pointer of the new node
    tail->SetNext(newNode); // Update the next pointer of the current tail
    tail = newNode;         // Update the tail to the new node
    size++;
}

template <typename T>
void TLinkedList<T>::InternPrepend(T aData) {
    TNode<T>* newNode = new TNode<T>(aData);
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


// Append(): Adds a new node with the given factory and returns the new node's data
template <typename T>
template <typename TArgs>
T TLinkedList<T>::Append(FDataFactory<T, TArgs> aDataFactory, TArgs aArgs) {
    if(aDataFactory == nullptr) {
		return nullptr; // Return nullptr if no factory is provided
	}
    T newData = aDataFactory(aArgs);
    InternAppend(newData);
    return newData;
}

template <typename T>
void TLinkedList<T>::Append(T aValue) {
    InternAppend(aValue);
}

// Prepend(): Adds a new node with the given factory to the beginning of the list
template <typename T>
template <typename TArgs>
T TLinkedList<T>::Prepend(FDataFactory<T, TArgs> aDataFactory, TArgs aArgs) {
	if (aDataFactory == nullptr) {
		return nullptr; // Return nullptr if no factory is provided
	}
    T newData = aDataFactory(aArgs);
    InternPrepend(newData);
	return newData;
}

template <typename T>
void TLinkedList<T>::Prepend(T aValue) {
    InternPrepend(aValue);
}

// GetAtIndex(): Returns the value at the specified index
template <typename T>
T TLinkedList<T>::GetAtIndex(int aIndex) {
    if (aIndex < 0 || aIndex >= size) {
        return nullptr; // Return nullptr if index is out of bounds
    }
    TNode<T>* current;
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
    return current->GetData(); // Return the data directly
}

// Remove(): Removes the first node with the given value
template <typename T>
void TLinkedList<T>::Remove(const T aValue) {
    TNode<T>* current = head->GetNext();
    while (current != nullptr) {
        if (current->GetData() == aValue) {
            TNode<T>* nodeToDelete = current;
            if (current->GetPrev() != nullptr) {
                current->GetPrev()->SetNext(current->GetNext());
            }
            if (current->GetNext() != nullptr) {
                current->GetNext()->SetPrev(current->GetPrev());
            }
            if (nodeToDelete == tail) {
                tail = current->GetPrev();
            }

			if (isDataOwner && nodeToDelete->GetData() != nullptr) {
				delete nodeToDelete->GetData(); // Delete the data if the list owns it
			}
            delete nodeToDelete;
            size--;
            return;
        }
        current = current->GetNext();
    }
}

// Contains(): Checks if the list contains the given value
template <typename T>
bool TLinkedList<T>::Contains(const T aValue) const {
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
T TLinkedList<T>::Search(const T aValue, FCheckNode<T> checkNode) const {
    TNode<T>* current = head->GetNext();
    while (current != nullptr) {
        if (checkNode == nullptr) {
            if (current->GetData() == aValue) {
                return current->GetData();
            }
        } else {
            if (checkNode(current->GetData(), aValue)) {
                return current->GetData();
            }
        }
        current = current->GetNext();
    }
    return nullptr;
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
        aVisitNode(static_cast<const T>(current->GetData()), index); // Cast to const T
        current = current->GetNext();
        index++;
    }
}

// Reverse(): Reverses the order of the nodes in the list
template <typename T>
void TLinkedList<T>::Reverse() {
    TNode<T>* current = head->GetNext();
    TNode<T>* temp = nullptr; // Temporary pointer for swapping

    // Handle an empty or single-node list
    if (current == nullptr || current->GetNext() == nullptr) {
        return;
    }

    // Iterate and swap next and prev pointers
    while (current != nullptr) {
        temp = current->GetPrev(); // Store prev pointer
        current->SetPrev(current->GetNext()); // Set prev to next
        current->SetNext(temp); // Set next to temp (which holds original prev)
        current = current->GetPrev(); // Move to the original next node
    }

    // Update head and tail
    TNode<T>* newHeadNext = tail; // The original tail becomes the new head->next
    tail = head->GetNext(); // The original first node is now the tail
    tail->SetNext(nullptr); // Ensure the new tail has no next pointer
    head->SetNext(newHeadNext); // Set the head's next to the new first node
}

template <typename T>
int TLinkedList<T>::GetSize() const {
    return size;
}
// --- End of TLinkedList class ---