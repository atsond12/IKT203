// Submission-01.cpp : Defines the entry point for the application.
//

#include "main.h"
#include <iostream> 



// Movie class with bitwise genre flags
enum EGenreFlags {
	Action =		0x0001,
	Comedy =		0x0002,
	Drama =			0x0004,
	Horror =		0x0008,
	SciFi =			0x0010,
	Romance =		0x0020,
	Documentary =	0x0040,
	Thriller =		0x0080,
	Crime =			0x0100,
	Fantasy =		0x0200,
	Animation =		0x0400,
	Adventure =		0x0800
};

static std::string GenreFlagsToString(int genreFlags) {
	std::string result;
	if (genreFlags & EGenreFlags::Action) result += "Action ";
	if (genreFlags & EGenreFlags::Comedy) result += "Comedy ";
	if (genreFlags & EGenreFlags::Drama) result += "Drama ";
	if (genreFlags & EGenreFlags::Horror) result += "Horror ";
	if (genreFlags & EGenreFlags::SciFi) result += "SciFi ";
	if (genreFlags & EGenreFlags::Romance) result += "Romance ";
	if (genreFlags & EGenreFlags::Documentary) result += "Documentary ";
	if (genreFlags & EGenreFlags::Thriller) result += "Thriller ";
	if (genreFlags & EGenreFlags::Crime) result += "Crime ";
	if (genreFlags & EGenreFlags::Fantasy) result += "Fantasy ";
	if (genreFlags & EGenreFlags::Animation) result += "Animation ";
	if (genreFlags & EGenreFlags::Adventure) result += "Adventure ";
	return result.empty() ? "None" : result;
}

// Movie class definition
class TMovie {
private:
	std::string title;
	std::string director;
	int year;
	int genreFlags; // Bitwise combination of EGenreFlags
	float rating; // Scale from 0.0 to 10.0

public:
	TMovie(std::string t, std::string d, int y, int g, float r)
		: title(t), director(d), year(y), genreFlags(g), rating(r) {}
	void PrintInfo() const {
		std::cout << "Title: " << title << "\nDirector: " << director
			<< "\nYear: " << year << "\nGenres: " << GenreFlagsToString(genreFlags)
			<< "\nRating: " << rating << "/10\n";
	}
	std::string GetTitle() const { return title; }
	std::string GetDirector() const { return director; }
	int GetYear() const { return year; }
	float GetRating() const { return rating; }
	int GetGenreFlags() const { return genreFlags; }
	bool HasGenre(EGenreFlags genre) const {
		return (genreFlags & genre) != 0;
	}
};

typedef bool (*FCheckMovie)(TMovie*, void*);
typedef void (*FMovieIndex)(TMovie*, int);

// Doubly Linked List TMovieNode definition
struct TMovieNode {
	TMovie* movie;
	TMovieNode* next;
	TMovieNode* prev;
	TMovieNode(TMovie* m) : movie(m), next(nullptr), prev(nullptr) {}
};

// Doubly Linked List class definition with dummy node, head, and tail O(1) operations
class TMovieList {
	private:
	TMovieNode* head; // Always points to dummy node 
	TMovieNode* tail;
	int size;

	// Helper function to get node at index
	TMovieNode* InternalGetAtIndex(int aIndex) {
		if (aIndex < 0 || aIndex >= size) return nullptr;
		TMovieNode* current;
		// Optimize traversal direction, if index is in the first half, start from head, else from tail
		if (aIndex < size / 2) {
			current = head->next; // Start from the beginning
			for (int i = 0; i < aIndex; i++) {
				current = current->next;
			}
		}
		else {
			current = tail; // Start from the end
			for (int i = size - 1; i > aIndex; i--) {
				current = current->prev;
			}
		}
		return current;
	}

public:
	TMovieList() {
		head = new TMovieNode(nullptr); // Dummy node
		tail = head; // Initially, tail is the same as head
		size = 0;
	}
	~TMovieList() {
		Clear();
		delete head; // Delete dummy node
	}
	void Clear() {
		TMovieNode* current = head->next; // Start from the first real node
		while (current) {
			TMovieNode* toDelete = current;
			current = current->next;
			delete toDelete->movie;
			delete toDelete;
		}
		head->next = nullptr;
		tail = head; // Reset tail to dummy node
		size = 0;
	}

	// Insertion at the end O(1)
	void Append(TMovie* aMovie) {
		TMovieNode* newNode = new TMovieNode(aMovie);
		newNode->prev = tail;
		tail->next = newNode;
		tail = newNode;
		size++;
	}

	// Prepend at the beginning O(1)
	void Prepend(TMovie* aMovie) {
		TMovieNode* newNode = new TMovieNode(aMovie);
		newNode->next = head->next;
		newNode->prev = head;
		if (head->next) {
			head->next->prev = newNode;
		} else {
			tail = newNode; // If list was empty, update tail
		}
		head->next = newNode;
		size++;
	}
	
	// GetAtIndex O(n) check direction to optimize
	TMovie* GetAtIndex(int aIndex) {
		return InternalGetAtIndex(aIndex)->movie;
	}

	// Remove at index O(n) use GetAtIndex to find node
	bool RemoveAtIndex(int aIndex) {
		if (aIndex < 0 || aIndex >= size) return false;
		TMovieNode* toDelte = InternalGetAtIndex(aIndex);
		if (!toDelte) return false;
		if (toDelte->prev) {
			toDelte->prev->next = toDelte->next;
		}
		if (toDelte->next) {
			toDelte->next->prev = toDelte->prev;
		} else {
			tail = toDelte->prev; // Update tail if last node is removed
		}
		delete toDelte->movie;
		delete toDelte;
		size--;
		return true;
	}

	// Reverse the list O(n)
	void Reverse() {
		TMovieNode* current = head->next;
		TMovieNode* prev = nullptr;
		tail = head->next;
		while (current) {
			TMovieNode* nextNode = current->next;
			current->next = prev;
			current->prev = nextNode;
			prev = current;
			current = nextNode;
		}
		head->next = prev;
		if (prev) {
			prev->prev = head;
		}
	}

	// SearchFor O(n)
	TMovie* SearchFor(FCheckMovie aCheckFunc, void* aUserData) {
		TMovieNode* current = head->next;
		while (current) {
			if (aCheckFunc(current->movie, aUserData)) {
				return current->movie;
			}
			current = current->next;
		}
		return nullptr;
	}

	// Every movie in the list O(n)
	void Every(FMovieIndex aIndexFunc) {
		TMovieNode* current = head->next;
		int index = 0;
		while (current) {
			aIndexFunc(current->movie, index);
			current = current->next;
			index++;
		}
	}

};

using namespace std;

static void PrintNode(std::string* data, int index) {
	cout << "Node " << index << ": " << *data << endl;
}

static bool CheckMovieByTitle(TMovie* movie, void* title) {
	return movie->GetTitle() == *(static_cast<std::string*>(title));
}

static bool CheckMovieByDirector(TMovie* movie, void* director) {
	return movie->GetDirector() == *(static_cast<std::string*>(director));
}

static bool FindAllMovieByGenre(TMovie* movie, void* genre) {
	if(movie->HasGenre(*(static_cast<EGenreFlags*>(genre)))) {
		movie->PrintInfo();
		std::cout << "-------------------" << std::endl;
	}
	// Always return false to continue searching
	return false;
}


int main()
{

	std::cout << "--- Submission 1: Linked List ---" << std::endl;

	// Create a movie list
	TMovieList movieList;
	// Add some movies
	movieList.Append(new TMovie("Inception", "Christopher Nolan", 2010, EGenreFlags::Action | EGenreFlags::SciFi, 8.8f));
	movieList.Append(new TMovie("The Godfather", "Francis Ford Coppola", 1972, EGenreFlags::Crime | EGenreFlags::Drama, 9.2f));
	movieList.Prepend(new TMovie("Toy Story", "John Lasseter", 1995, EGenreFlags::Animation | EGenreFlags::Adventure | EGenreFlags::Comedy, 8.3f));
	movieList.Append(new TMovie("The Dark Knight", "Christopher Nolan", 2008, EGenreFlags::Action | EGenreFlags::Crime | EGenreFlags::Drama, 9.0f));
	// Print movie info
	for (int i = 0; i < 3; i++) {
		TMovie* movie = movieList.GetAtIndex(i);
		if (movie) {
			movie->PrintInfo();
			std::cout << "-------------------" << std::endl;
		}
	}
	std::cout << std::endl;
	// Wait for user input to proceed
	std::cout << "Press Enter to continue..." << std::endl;
	std::cin.get();

	// Search for a movie by title
	std::string searchTitle = "Inception";
	TMovie* foundMovie = movieList.SearchFor(CheckMovieByTitle, &searchTitle);
	if (foundMovie) {
		std::cout << "Found movie by title '" << searchTitle << "':" << std::endl;
		foundMovie->PrintInfo();
	} else {
		std::cout << "Movie with title '" << searchTitle << "' not found." << std::endl;
	}
	std::cout << "-------------------" << std::endl;
	std::cout << std::endl;

	// Search for a movie by director
	std::string searchDirector = "John Lasseter";
	foundMovie = movieList.SearchFor(CheckMovieByDirector, &searchDirector);
	if (foundMovie) {
		std::cout << "Found movie by director '" << searchDirector << "':" << std::endl;
		foundMovie->PrintInfo();
	} else {
		std::cout << "Movie with director '" << searchDirector << "' not found." << std::endl;
	}
	std::cout << "-------------------" << std::endl;
	std::cout << std::endl;

	// Find all movies in the Action genre
	EGenreFlags searchGenre = EGenreFlags::Action;
	std::cout << "Movies in the Action genre:" << std::endl;
	movieList.SearchFor(FindAllMovieByGenre, &searchGenre);
	std::cout << std::endl;

	// Reverse the list
	movieList.Reverse();
	std::cout << "Movies after reversing the list:" << std::endl;
	movieList.Every([](TMovie* movie, int index) {
		std::cout << "Index " << index << ":" << std::endl;
		movie->PrintInfo();
		std::cout << "-------------------" << std::endl;
		});

	return 0;
}
