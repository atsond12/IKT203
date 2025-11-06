#pragma once
#include "song.h"

class TSongListNode {
public:
	TSong* song;
	TSongListNode* prev;
	TSongListNode* next;

	TSongListNode(TSong* aSong)
		: song(aSong), prev(nullptr), next(nullptr) {
	}
};

class TSongList {
private:
	TSongListNode* head;
	TSongListNode* tail;
	bool isDataOwner; // Responsible for deleting data

	void Clear();
public:
	TSongList(bool aIsDataOwner);
	~TSongList();
	bool IsEmpty() const;

	void Append(TSong* aSong);
	TSong* GetFirstSong() const;
	TSong* GetNextSong(TSong* aCurrentSong) const;
	TSong* GetPreviousSong(TSong* aCurrentSong) const;

};