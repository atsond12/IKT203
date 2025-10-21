#include "BankAccountList.h"

TLinkedList::TLinkedList(bool aOwnsData) : head(nullptr), ownsData(aOwnsData), size(0) {
	head = new TLinkedListNode(nullptr); // Dummy head node
}

TLinkedList::~TLinkedList()
{
	while (head->next != nullptr)
	{
		TLinkedListNode* temp = head->next;
		head->next = temp->next;
		if (ownsData) delete temp->data; // Delete the TBankAccount object
		delete temp; // Delete the node
	}
	delete head;
}

int TLinkedList::getSize() const { return size; }

void TLinkedList::Add(TBankAccount* aData)
{
	TLinkedListNode* newNode = new TLinkedListNode(aData);
	newNode->next = head->next;
	head->next = newNode;
	size++;
}

TBankAccount* TLinkedList::Find(FCompareAccount aCompareFunc, void* aSearchKey)
{
	TLinkedListNode* current = head->next;
	while (current != nullptr)
	{
		if (aCompareFunc(current->data, aSearchKey))
		{
			return current->data; // Found
		}
		current = current->next;
	}
	return nullptr; // Not found
}

TLinkedList* TLinkedList::Every(FCompareAccount aCompareFunc, void* aSearchKey)
{
	TLinkedList* resultList = new TLinkedList(false); // New list does not own data
	TLinkedListNode* current = head->next;
	while (current != nullptr)
	{
		if (aCompareFunc(current->data, aSearchKey))
		{
			resultList->Add(current->data); // Add to result list
		}
		current = current->next;
	}
	return resultList; // Return the new list
}

TBankAccount** TLinkedList::ToArray()
{
	if (size == 0) return nullptr;
	TBankAccount** array = new TBankAccount * [size];
	TLinkedListNode* current = head->next;
	int index = 0;
	while (current != nullptr && index < size) // Ensure index < size
	{
		array[index++] = current->data;
		current = current->next;
	}
	return array;
}

void TLinkedList::forEach(FForEachAccount aFunc)
{
	TLinkedListNode* current = head->next;
	int index = 0;
	while (current != nullptr)
	{
		aFunc(current->data, index++);
		current = current->next;
	}
}

TLinkedListNode* TLinkedList::getHead() const { return head; }

void TLinkedList::Remove(TBankAccount* account)
{
	TLinkedListNode* current = head;
	while (current->next != nullptr)
	{
		if (current->next->data == account)
		{
			TLinkedListNode* temp = current->next;
			current->next = temp->next;
			if (ownsData) delete temp->data; // Delete the TBankAccount object
			delete temp; // Delete the node
			size--;
			return; // Exit after removing
		}
		current = current->next;
	}
}


