// Option 2 (Advanced): Console Music Player.

#include <iostream>
#include "option2.h"
#include "SharedLib.h"
#include "song.h"
#include "songList.h"

static TSongList* songs = nullptr;
static TSongQueue* playQueue = nullptr;
static TSongQueue* wishQueue = nullptr;

static bool SongReadCallback(const int aIndex, const int aTotalCount, const std::string& aArtist, const std::string& aTitle, const std::string& aYear, const std::string& aGenre, const std::string& aSource) {
	TSong* song = new TSong(aIndex, aArtist, aTitle, aYear, aGenre, aSource);
	//song->printOut();
	songs->Append(song);
	return true;
}


int RunApp() {
	// Implement the Console Music Player application logic here

	std::string filename = "F:\\IKT203\\VisualStudio\\DATA\\songs.txt"; // Specify the path to the songs file

	songs = new TSongList(true); // Create a song list that owns its data

	ReadSongsFromFile(filename, SongReadCallback);

	// Example: Print out all songs. forward and backward
	TSong* currentSong = songs->GetFirstSong();
	std::cout << "Songs in forward order:\r\n";
	while (1) {
		currentSong->printOut();
		TSong* temp = songs->GetNextSong(currentSong);
		if (temp == nullptr) break;
		currentSong = temp;
	}
	std::cout << "Songs in backward order:\r\n";
	// currentSong is now at the last song
	while (1) {
		currentSong->printOut();
		TSong* temp = songs->GetPreviousSong(currentSong);
		if (temp == nullptr) break;
		currentSong = temp;
	}

	//We add the 5 first songs to the play queue
	playQueue = new TSongQueue(); // Create a play queue that does not own its data
	currentSong = songs->GetFirstSong();
	for (int i = 0; i < 5; i++) {
		playQueue->Append(currentSong);
		currentSong = songs->GetNextSong(currentSong);
	}
	//We add the 4 next songs to the wish queue
	wishQueue = new TSongQueue(); // Create a wish queue that does not own its data
	for (int i = 0; i < 4; i++) {
		wishQueue->Append(currentSong);
		currentSong = songs->GetNextSong(currentSong);
	}

	std::cout << "\r\nPlay Queue:\r\n";
	currentSong = playQueue->GetFirstSong();
	while (currentSong != nullptr) {
		currentSong->printOut();
		currentSong = playQueue->GetNextSong(currentSong);
	}
	std::cout << "\r\nWish Queue:\r\n";
	currentSong = wishQueue->GetFirstSong();
	while (currentSong != nullptr) {
		currentSong->printOut();
		currentSong = wishQueue->GetNextSong(currentSong);
	}

	// Play all songs in the play queue, and remove them from the play queue
	std::cout << "\r\nPlaying songs from the Play Queue:\r\n";
	while (playQueue->IsEmpty() == false) {
		if (wishQueue->IsEmpty() == false) {
			// Move the first song from the wish queue to the play queue
			TSong* wishSong = wishQueue->GetFirstSong();
			std::cout << "\r\nPlaing from Wish Queue:\r\n";
			wishSong->printOut();
			wishQueue->Dequeue();
		}
		else {
			TSong* songToPlay = playQueue->GetFirstSong();
			std::cout << "\r\nNow playing:\r\n";
			songToPlay->printOut();
			// Remove the song from the play queue
			playQueue->Dequeue();
		}
	}

	delete songs; // Clean up the song list and its data
	return 0;
}