#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Common.h"

void readGraphFromFile(const string& aFilename, FNodeRead aOnNodeRead, FEdgeRead aOnEdgeRead) {
	if (aFilename.empty()) return;

	std::ifstream file(aFilename);
	if (!file.is_open()) return;

	std::string line;
	bool readingNodes = true;

	while (std::getline(file, line)) {
		if (line == "[NODES]") {
			readingNodes = true;
			continue;
		}
		else if (line == "[EDGES]") {
			readingNodes = false;
			continue;
		}

		if (readingNodes) {
			if (!aOnNodeRead(line)) break;
		}
		else {
			std::istringstream iss(line);
			string fromNode, toNode;
			float weight;
			if (std::getline(iss, fromNode, ';') &&
				std::getline(iss, toNode, ';') &&
				(iss >> weight)) {
				if (!aOnEdgeRead(fromNode, toNode, weight)) break;
			}
		}
	}

	file.close();
}


// Example usage with function:
/*
bool onNodeRead(string& aNode) {
	std::cout << "Node: " << aNode << std::endl;
	return true; // Continue reading
}

bool onEdgeRead(string& aFromNode, string& aToNode, float aWeight) {
	std::cout << "Edge from " << aFromNode << " to " << aToNode << " with weight " << aWeight << std::endl;
	return true; // Continue reading
}

int main() {
	readGraphFromFile("graph.txt", onNodeRead, onEdgeRead);
	return 0;
}
*/

// Example usage with lambda:
/*
int main() {
	readGraphFromFile("graph.txt",
		[](string& aNode) {
			std::cout << "Node: " << aNode << std::endl;
			return true; // Continue reading
		},
		[](string& aFromNode, string& aToNode, float aWeight) {
			std::cout << "Edge from " << aFromNode << " to " << aToNode << " with weight " << aWeight << std::endl;
			return true; // Continue reading
		}
	);
	return 0;
}
*/