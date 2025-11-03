#pragma once
#include <string>

struct TSong
{
	std::string title;
	std::string artist;

	TSong(std::string aTitle, std::string aArtist)
		: title(aTitle), artist(aArtist) {
	}
};

// TMusicPlayerApp.h
#pragma once
#include "TLinkedList.hpp"
#include "TQueue.hpp"
#include "TStack.hpp"
#include "TSong.h"
#include <string>

class TMusicPlayerApp
{
private:
	// Attributes
	TLinkedList<TSong*> mSongLibrary;
	TQueue<TSong*>      mMainQueue;
	TQueue<TSong*>      mWishQueue;
	TStack<TSong*>      mHistoryStack;

	// Private helper for loading
	void LoadLibrary(const std::string& aFilename);

public:
	// Constructor / Destructor
	TMusicPlayerApp(const std::string& aSongFilename);
	~TMusicPlayerApp(); // Will be empty, destructors of attributes handle it

	// Public API for RunApp() to call
	// ... (we will define these next)
};