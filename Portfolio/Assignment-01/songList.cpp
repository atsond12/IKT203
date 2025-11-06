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
			//std::cout << "Deleted song data." << std::endl;
		}
		delete current;
		current = nextNode;
		//std::cout << "Deleted song node." << std::endl;
	}
	head->next = tail;
	tail->prev = head;
}

bool TSongList::IsEmpty() const {
	return head->next == tail;
}

void TSongList::Append(TSong* aSong) {
	TSongListNode* newNode = new TSongListNode(aSong);
	newNode->prev = tail->prev;
	newNode->next = tail;
	tail->prev->next = newNode;
	tail->prev = newNode;
}

TSong* TSongList::GetFirstSong() const {
	if (IsEmpty()) {
		return nullptr; // List is empty
	}
	return head->next->song;
}

// This has time complexity O(n)
// You can use TSongListNode and get a time complexity O(1)
TSong* TSongList::GetNextSong(TSong* aCurrentSong) const {
	if (IsEmpty()) {
		return nullptr; // List is empty
	}

	TSongListNode* currentNode = head->next;
	while (currentNode != tail) {
		if (currentNode->song == aCurrentSong) {
			if (currentNode->next != tail) {
				return currentNode->next->song;
			}
			else {
				return nullptr; // No next song
			}
		}
		currentNode = currentNode->next;
	}
	return nullptr; // Current song not found
}

// This has time complexity O(n)
// You can use TSongListNode and get a time complexity O(1)
TSong* TSongList::GetPreviousSong(TSong* aCurrentSong) const {
	if (IsEmpty()) {
		return nullptr; // List is empty
	}
	TSongListNode* currentNode = head->next;
	while (currentNode != tail) {
		if (currentNode->song == aCurrentSong) {
			if (currentNode->prev != head) {
				return currentNode->prev->song;
			}
			else {
				return nullptr; // No previous song
			}
		}
		currentNode = currentNode->next;
	}
	return nullptr; // Current song not found
}


TSongQueue::TSongQueue() : TSongList(false) {} // Queue does not own its data

void TSongQueue::Enqueue(TSong* aSong) {
	Append(aSong);
}

TSong* TSongQueue::Dequeue() {
	TSong* firstSong = GetFirstSong();
	if (firstSong != nullptr) {
		// This calss is friend of TSongList, so it can access private members
		TSongListNode* firstNode = head->next;
		head->next = firstNode->next;
		firstNode->next->prev = head;
		delete firstNode;
	}
	return firstSong;
}

