// Option 2 (Advanced): Console Music Player.

#include <iostream>
#include "option2.h"
#include "SharedLib.h"

static bool SongReadCallback(const int aIndex, const int aTotalCount, const std::string& aArtist, const std::string& aTitle, const std::string& aYear, const std::string& aGenre, const std::string& aSource) {
	// Implement the logic to process each song read from the file
	// For example, print the song details to the console
	std::cout << "Song " << (aIndex + 1) << " of " << aTotalCount << ":\n";
	std::cout << "  Artist: " << aArtist << "\n";
	std::cout << "  Title: " << aTitle << "\n";
	std::cout << "  Year: " << aYear << "\n";
	std::cout << "  Genre: " << aGenre << "\n";
	std::cout << "  Source: " << aSource << "\n\n";
	// Return true to continue reading more songs
	return true;
}


int RunApp() {
	// Implement the Console Music Player application logic here

	std::string filename = "F:\\IKT203\\VisualStudio\\DATA\\songs.txt"; // Specify the path to the songs file
	ReadSongsFromFile(filename, SongReadCallback);

	return 0;
}