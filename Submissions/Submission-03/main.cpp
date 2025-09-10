// Submission-01.cpp : Defines the entry point for the application.
//

#include "main.h"
#include <iostream> 


/*
Part 1: Implementing the Core Data Structures
These tasks are designed to get you comfortable with the Last-In, First-Out (LIFO) and First-In, First-Out (FIFO) principles. Building these yourself will give you a deep understanding of how they work under the hood!
*/

/*
1. Implementing a Stack:
a) Create a simple TStack class that can hold int values.
b) Use a fixed-size array and a top-of-stack index to manage the data.
c) Implement the core methods: Push(int item) and Pop().
D) Add a Peek() method to view the top item without removing it.
e) Include an IsEmpty() method to check if the stack is empty.
*/

class TStackArray {
private:
	int maxSize = 0;
	int* stackArray = nullptr;
	int top = -1; // Index of the top element
public:
	TStackArray(int aSize) : maxSize(aSize) {
		stackArray = new int[maxSize];
	}
	~TStackArray() {
		delete[] stackArray;
	}
	void Push(int aItem) {
		if (top < maxSize - 1) {
			stackArray[++top] = aItem;
		}
		else {
			std::cout << "Stack Overflow" << std::endl;
		}
	}
	int Pop() {
		if (!IsEmpty()) {
			return stackArray[top--];
		}
		else {
			std::cout << "Stack Underflow" << std::endl;
			return -1; // Indicate error
		}
	}
	int Peek() const {
		if (!IsEmpty()) {
			return stackArray[top];
		}
		else {
			std::cout << "Stack is empty" << std::endl;
			return -1; // Indicate error
		}
	}
	bool IsEmpty() const {
		return top == -1;
	}
};


/*
2. Implementing a Queue:
a) Create a simple TQueue class that can hold int values.
b) Use a fixed-size array and front/back indices to manage the data.
c) Implement the core methods: Enqueue(int item) and Dequeue().
d) Add a Peek() method to view the item at the front without removing it.
e) Include an IsEmpty() method to check if the queue is empty.
*/

class TQueueArray {
private:
	int maxSize = 0;
	int* queueArray = nullptr;
	int front = 0; // Index of the front element
	int back = -1; // Index of the back element
	int itemCount = 0; // Number of items in the queue
public:
	TQueueArray(int aSize) : maxSize(aSize) {
		queueArray = new int[maxSize];
	}
	~TQueueArray() {
		delete[] queueArray;
	}
	void Enqueue(int aItem) {
		if (itemCount < maxSize) {
			back = (back + 1) % maxSize; // Circular increment
			queueArray[back] = aItem;
			itemCount++;
		}
		else {
			std::cout << "Queue Overflow" << std::endl;
		}
	}
	int Dequeue() {
		if (!IsEmpty()) {
			int item = queueArray[front];
			front = (front + 1) % maxSize; // Circular increment
			itemCount--;
			return item;
		}
		else {
			std::cout << "Queue Underflow" << std::endl;
			return -1; // Indicate error
		}
	}
	int Peek() const {
		if (!IsEmpty()) {
			return queueArray[front];
		}
		else {
			std::cout << "Queue is empty" << std::endl;
			return -1; // Indicate error
		}
	}
	bool IsEmpty() const {
		return itemCount == 0;
	}
};

class TNodeInteger {
	public:
	int data;
	TNodeInteger* next;
	TNodeInteger(int aData) : data(aData), next(nullptr) {}
};

// Stack implemented using a linked list with dummy head node
class TStackLinkedList {
	private:
		TNodeInteger* top = nullptr;
public:
	TStackLinkedList() {
		top = new TNodeInteger(0); // Dummy head node
	}
	~TStackLinkedList() {
		while (!IsEmpty()) {
			Pop();
		}
		delete top; // Delete dummy head node
	}
	void Push(int aItem) {
		TNodeInteger* newNode = new TNodeInteger(aItem);
		newNode->next = top->next;
		top->next = newNode;
	}
	int Pop() {
		if (!IsEmpty()) {
			TNodeInteger* temp = top->next;
			int item = temp->data;
			top->next = temp->next;
			delete temp; // Free memory
			return item; 
		}
		else {
			std::cout << "Stack Underflow" << std::endl;
			return -1; // Indicate error
		}
	}
	int Peek() const {
		if (!IsEmpty()) {
			return top->next->data;
		}
		else {
			std::cout << "Stack is empty" << std::endl;
			return -1; // Indicate error
		}
	}
	bool IsEmpty() const {
		return top->next == nullptr;
	}
};

// Queue implemented using a linked list with dummy head node
class TQueueLinkedList {
private:
	TNodeInteger* front = nullptr;
	TNodeInteger* back = nullptr;
public:
	TQueueLinkedList() {
		front = new TNodeInteger(0); // Dummy head node
		back = front; // Initially, front and back point to the dummy node
	}
	~TQueueLinkedList() {
		while (!IsEmpty()) {
			Dequeue();
		}
		delete front; // Delete dummy head node
	}
	void Enqueue(int aItem) {
		TNodeInteger* newNode = new TNodeInteger(aItem);
		back->next = newNode;
		back = newNode;
	}
	int Dequeue() {
		if (!IsEmpty()) {
			TNodeInteger* temp = front->next;
			int item = temp->data;
			front->next = temp->next;
			if (back == temp) { // If the dequeued node was the last node
				back = front; // Reset back to the dummy head
			}
			delete temp; // Free memory
			return item;
		}
		else {
			std::cout << "Queue Underflow" << std::endl;
			return -1; // Indicate error
		}
	}
	int Peek() const {
		if (!IsEmpty()) {
			return front->next->data;
		}
		else {
			std::cout << "Queue is empty" << std::endl;
			return -1; // Indicate error
		}
	}
	bool IsEmpty() const {
		return front->next == nullptr;
	}
};



/*
Part 2: Practical Applications
Now that you have your own data structures, it's time to put them to work! These are classic problems that perfectly demonstrate the LIFO and FIFO principles.
*/

/*
3. String Reversal with a Stack:
a) Write a function that takes a string as input and uses your TStack to return the reversed string.
b) In a short comment, explain why the stack is the perfect tool for this type of task.
*/

static std::string ReverseString(const char* aStr) {
	TStackArray stack(strlen(aStr));
	for (int i = 0; aStr[i] != '\0'; i++) {
		stack.Push(aStr[i]);
	}
	std::string reversed;
	while (!stack.IsEmpty()) {
		reversed += static_cast<char>(stack.Pop()); // Cast int back to char
	}
	return reversed;
	/*
	* Note: The stack is the perfect tool for string reversal because it allows us to push each character of the string onto the stack and then pop them off in reverse order.
	* And stack is using the rule of LIFO (Last In First Out), so the last character pushed onto the stack will be the first one to be popped off, effectively reversing the order of characters.
	*/
}

/*
4. Recursive Functions with a Stack:
a) Remember the factorial function you implemented with recursion in Submission 2? Your computer used a hidden "call stack" to make that happen. Now, your task is to re-implement that function without recursion, using your own `TStack` to manage the process.
b) This is a fantastic exercise that will give you a "eureka" moment about how recursion truly works!
*/

static int Factorial(int n) {
	if (n < 0) return -1; // Factorial is not defined for negative numbers
	if (n == 0 || n == 1) return 1; // Base case
	TStackArray stack(n);
	for (int i = 2; i <= n; i++) {
		stack.Push(i);
	}
	int result = 1;
	while (!stack.IsEmpty()) {
		result *= stack.Pop();
	}
	return result;
}

/*
5. Wait Line Simulation with a Queue:
a) Simulate a simple waiting line, such as a ticket counter.
b) Use your TQueue to manage a list of people (represented by integer IDs).
c) People should Enqueue when they arrive and Dequeue when they are served, clearly demonstrating the FIFO principle.
*/

static void SimulateWaitLine() {
	TQueueArray queue(5); // Queue with a maximum size of 5
	// Simulate people arriving
	for (int i = 1; i <= 5; i++) {
		std::cout << "Person " << i << " arrives." << std::endl;
		queue.Enqueue(i);
	}
	// Simulate serving people
	while (!queue.IsEmpty()) {
		int person = queue.Dequeue();
		std::cout << "Person " << person << " is served." << std::endl;
	}
}

/*
Part 3: Advanced Traversal - Stacks vs. Queues
This is the main event! You will solve the same problem using two different approaches, highlighting the different strengths of stacks and queues.
Your recursive functions from Submission 2 actually use an implicit stack, so this task will give you a deeper look into how it all works.
*/

/*
6. Setup: The 100x100 Grid:
a) Create a 100x100 two-dimensional integer array.
b) Populate the array with random integer values between 0 and 9.
c) Choose a random starting cell (row, col).
d) Create a second 100x100 boolean array to keep track of visited cells.
*/


static const int GRID_SIZE = 7;
static int grid[GRID_SIZE][GRID_SIZE];
static bool visited[GRID_SIZE][GRID_SIZE] = { false };
#include <cstdlib>
#include <ctime>
static void InitializeGrid() {
	std::srand(static_cast<unsigned int>(std::time(0))); // Seed for randomness
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			grid[i][j] = std::rand() % 9; // Random values between 0 and 9
			visited[i][j] = false; // Initialize visited array
		}
	}
}

/*
7. Depth-First Search (DFS) with a Stack:
a) Write a function that uses your TStack to perform a DFS on the grid, starting from your random cell.
b) The goal is to find the first occurrence of the number '0'.
c) In a short comment, explain how the LIFO behavior of the stack guides the search to explore as deeply as possible along one path before backtracking.
*/


// Helper function to check if a cell is within bounds and not visited
static bool IsValid(int aRow, int aCol) {
	return aRow >= 0 && aRow < GRID_SIZE && aCol >= 0 && aCol < GRID_SIZE && !visited[aRow][aCol];
}

// The stack's LIFO behavior allows the DFS to explore one path fully before backtracking, ensuring that all possible routes are checked in depth-first order.
static bool DFS(int aStartRow, int aStartCol) {
	TStackArray stack(GRID_SIZE * GRID_SIZE);
	int cellPos = aStartRow * GRID_SIZE + aStartCol; // Encode 2D position as 1D
	stack.Push(cellPos); 
	while (!stack.IsEmpty()) {
		cellPos = stack.Pop();
		int row = cellPos / GRID_SIZE;
		int col = cellPos % GRID_SIZE;
		if (grid[row][col] == 0) {
			std::cout << "Found 0 at (" << row << ", " << col << ")" << std::endl;
			return true;
		}
		std::cout << "Visiting (" << row << ", " << col << ") with value " << grid[row][col] << std::endl;
		visited[row][col] = true;
		int neighbors[4][2] = {
			{row - 1, col},     // Top
			{row, col + 1},     // Right
			{row + 1, col},     // Bottom
			{row, col - 1}      // Left
		};

		for(int i = 0; i < 4; i++) {
			int newRow = neighbors[i][0];
			int newCol = neighbors[i][1];
			if (IsValid(newRow, newCol)) {
				cellPos = newRow * GRID_SIZE + newCol;
				stack.Push(cellPos);
			}
		}
	}
	std::cout << "0 not found in DFS" << std::endl;
	return false;
}

// The queue's FIFO behavior ensures that the BFS explores all neighbors at the present depth prior to moving on to nodes at the next depth level, effectively searching layer by layer.
static bool BFS(int aStartRow, int aStartCol) {
	TQueueArray queue(GRID_SIZE * GRID_SIZE);
	queue.Enqueue(aStartRow * GRID_SIZE + aStartCol); // Encode 2D position as 1D
	while (!queue.IsEmpty()) {
		int pos = queue.Dequeue();
		int row = pos / GRID_SIZE;
		int col = pos % GRID_SIZE;
		if (grid[row][col] == 0) {
			std::cout << "Found 0 at (" << row << ", " << col << ")" << std::endl;
			return true;
		}
		std::cout << "Visiting (" << row << ", " << col << ") with value " << grid[row][col] << std::endl;
		visited[row][col] = true;
		// Push adjacent cells onto the queue  (top-left, top, top-right, right, bottom-right, bottom, bottom-left, left)
		
		int neighbors[4][2] = {
			{row - 1, col},     // Top
			{row, col + 1},     // Right
			{row + 1, col},     // Bottom
			{row, col - 1}      // Left
		};
		for(int i = 0; i < 4; i++) {
			int newRow = neighbors[i][0];
			int newCol = neighbors[i][1];
			if (IsValid(newRow, newCol)) {
				queue.Enqueue(newRow * GRID_SIZE + newCol);
			}
		}


	}
	std::cout << "0 not found in BFS" << std::endl;
	return false;
}


int main()
{
	std::cout << "--- Submission 3: Stacks & Queues ---" << std::endl;
	std::string original = "Hello, World!";
	std::string reversed = ReverseString(original.c_str());
	std::cout << "Original String: " << original << std::endl;
	std::cout << "Reversed String: " << reversed << std::endl;
	// The stack is the perfect tool for string reversal because it operates on a Last-In, First-Out (LIFO) principle.
	// This means that the last character pushed onto the stack will be the first one to be popped off, effectively reversing the order of characters.
	std::cout << "----------------------------------------------------" << std::endl << std::endl;

	std::cout << "Calculating Factorial of 5 using Stack:" << std::endl;
	std::cout << "5! = " << Factorial(5) << std::endl;
	std::cout << "----------------------------------------------------" << std::endl << std::endl;

	std::cout << "Simulating Wait Line using Queue:" << std::endl;
	SimulateWaitLine();
	std::cout << "----------------------------------------------------" << std::endl << std::endl;

	std::cout << "Initializing 100x100 Grid and Performing DFS to find '0':" << std::endl;
	InitializeGrid();
	int startRow = 3; //std::rand() % GRID_SIZE;
	int startCol = 2; //std::rand() % GRID_SIZE;
	std::cout << "Starting DFS from (" << startRow << ", " << startCol << ")" << std::endl;
	DFS(startRow, startCol);
	std::cout << std::endl << "Re-initializing visited array for BFS:" << std::endl;
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			visited[i][j] = false; // Reset visited array
		}
	}
	std::cout << "Stating BFS from (" << startRow << ", " << startCol << ")" << std::endl;
	BFS(startRow, startCol); // This should find a different path to '0'
	std::cout << "----------------------------------------------------" << std::endl << std::endl;


	return 0;
}
