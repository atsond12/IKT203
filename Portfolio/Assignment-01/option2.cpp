// Option 2 (Advanced): Console Music Player.

#include <iostream>
#include "option2.h"
#include "SharedLib.h"
#include "song.h"
#include "songList.h"

static

static bool SongReadCallback(const int aIndex, const int aTotalCount, const std::string& aArtist, const std::string& aTitle, const std::string& aYear, const std::string& aGenre, const std::string& aSource) {
	TSong* song = new TSong(aIndex, aArtist, aTitle, aYear, aGenre, aSource);
	song->printOut();
	delete song;// Clean up memory after use, only for test purposes
	return true;
}


int RunApp() {
	// Implement the Console Music Player application logic here

	std::string filename = "F:\\IKT203\\VisualStudio\\DATA\\songs.txt"; // Specify the path to the songs file
	ReadSongsFromFile(filename, SongReadCallback);

	return 0;
}