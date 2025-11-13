#pragma once
/**
	* @brief TLinkedListNode class represents a node in a linked list.
*/
template <typename T>
class TLinkedListNode
{
private:
	T data;
	TLinkedListNode* prev;
	TLinkedListNode* next;
public:
	TLinkedListNode(const T& data)
		: data(data), prev(nullptr), next(nullptr) {
	}

	void SwapNextPrev()
	{
		TLinkedListNode* temp = next;
		next = prev;
		prev = temp;
	}

	friend class TLinkedList<T>;
};

/*
	*@brief TLinkedList class represents a doubly linked list.
*/
template <typename T>
class TLinkedList
{
private:
	// We need big O(1) for adding/removing from head/tail
	TLinkedListNode<T>* head;
	TLinkedListNode<T>* tail;
	int count;
	bool isDataOwner; // Indicates if the list owns the data and should delete it

	void Clear() {
		int deletedNodes = 0;
		int deletedData = 0;
		TLinkedListNode<T>* current = head->next;
		while (current != tail) {
			TLinkedListNode<T>* nodeToDelete = current;
			current = current->next;
			if (isDataOwner) {
				delete nodeToDelete->data;
				deletedData++;
			}
			delete nodeToDelete;
			deletedNodes++;
		}
		head->next = tail;
		tail->prev = head;
	}

public:
	TLinkedList(bool ownsData = false)
		: head(nullptr), tail(nullptr), count(0), isDataOwner(ownsData) {
		//  We are using dummy head and tail nodes to simplify add/remove logic
		head = new TLinkedListNode<T>(T());
		tail = new TLinkedListNode<T>(T());
		head->next = tail;
		tail->prev = head;
	}

	~TLinkedList() {
		Clear();
		delete head;
		delete tail;
	}

	bool isEmpty() const {
		return count == 0;
	}

	void Append(const T& aData) {
		TLinkedListNode<T>* newNode = new TLinkedListNode<T>(aData);
		newNode->prev = tail->prev;
		newNode->next = tail;
		tail->prev->next = newNode;
		tail->prev = newNode;
		count++;
	}

	void Prepend(const T& aData) {
		TLinkedListNode<T>* newNode = new TLinkedListNode<T>(aData);
		newNode->next = head->next;
		newNode->prev = head;
		head->next->prev = newNode;
		head->next = newNode;
		count++;
	}

	T RemoveHead() {
		if (isEmpty()) throw std::runtime_error("List is empty");
		TLinkedListNode<T>* nodeToRemove = head->next;
		T data = nodeToRemove->data;
		// Re-link the list
		TLinkedListNode<T>* nextNode = nodeToRemove->next;
		head->next = nextNode;
		nextNode->prev = head;
		delete nodeToRemove;
		count--;
		// Users are responsible for deleting data if isDataOwner is true
		return data;
	}

	T RemoveTail() {
		if (isEmpty()) throw std::runtime_error("List is empty");
		TLinkedListNode<T>* nodeToRemove = tail->prev;
		T data = nodeToRemove->data;
		// Re-link the list
		TLinkedListNode<T>* prevNode = nodeToRemove->prev;
		tail->prev = prevNode;
		prevNode->next = tail;
		delete nodeToRemove;
		count--;
		// Users are responsible for deleting data if isDataOwner is true
		return data;
	}

};