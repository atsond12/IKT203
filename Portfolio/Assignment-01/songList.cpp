#include "songList.h"

TSongList::TSongList(bool aIsDataOwner)
	: head(nullptr), tail(nullptr), isDataOwner(aIsDataOwner) {
	// This list use dummy head and tail nodes for easier management
	head = new TSongListNode(nullptr);
	tail = new TSongListNode(nullptr);
	head->next = tail;
	tail->prev = head;
}

TSongList::~TSongList()
{
	Clear();
	delete head;
	delete tail;
}

void TSongList::Clear() {
	TSongListNode* current = head->next;
	while (current != tail) {
		TSongListNode* nextNode = current->next;
		if (isDataOwner && current->song != nullptr) {
			delete current->song;
			std::cout << "Deleted song data." << std::endl;
		}
		delete current;
		current = nextNode;
		std::cout << "Deleted song node." << std::endl;
	}
	head->next = tail;
	tail->prev = head;
}

void TSongList::Append(TSong* aSong) {
	TSongListNode* newNode = new TSongListNode(aSong);
	newNode->prev = tail->prev;
	newNode->next = tail;
	tail->prev->next = newNode;
	tail->prev = newNode;
}
