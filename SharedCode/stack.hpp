#pragma once
#include <stdexcept> // For std::runtime_error

// The template now takes a type 'T' and an integer 'MAX_SIZE'
template <typename T, int MAX_SIZE>
class TStack {
private:
    T data[MAX_SIZE]; // A static array of T with MAX_SIZE elements
    int topIndex;     // The index of the top element (-1 if empty)

public:
    // Constructor
    TStack();

    // No destructor is needed since we aren't using 'new'

    // --- Core Stack Operations ---
    void Push(const T& aElement);
    T Pop();
    T Peek() const;

    // --- Helper Functions ---
    int GetSize() const;
    bool IsEmpty() const;
    bool IsFull() const;
};

// --- Method Implementations ---

// Constructor: Just initializes the top index
template <typename T, int MAX_SIZE>
TStack<T, MAX_SIZE>::TStack() : topIndex(-1) {
    // No memory allocation is needed
}

// push(): Adds an element to the top
template <typename T, int MAX_SIZE>
void TStack<T, MAX_SIZE>::Push(const T& aElement) {
    if (IsFull()) {
        throw std::runtime_error("Stack overflow: cannot push to a full stack.");
    }
    data[++topIndex] = aElement;
}

// pop(): Removes and returns the top element
template <typename T, int MAX_SIZE>
T TStack<T, MAX_SIZE>::Pop() {
    if (IsEmpty()) {
        throw std::runtime_error("Stack underflow: cannot pop from an empty stack.");
    }
    return data[topIndex--];
}

// peek(): Returns the top element without removing it
template <typename T, int MAX_SIZE>
T TStack<T, MAX_SIZE>::Peek() const {
    if (IsEmpty()) {
        throw std::runtime_error("Stack is empty: cannot peek.");
    }
    return data[topIndex];
}

// size(): Returns the current number of elements
template <typename T, int MAX_SIZE>
int TStack<T, MAX_SIZE>::GetSize() const {
    return topIndex + 1;
}

// isEmpty(): Checks if the stack is empty
template <typename T, int MAX_SIZE>
bool TStack<T, MAX_SIZE>::IsEmpty() const {
    return topIndex == -1;
}

// isFull(): Checks if the stack is full
template <typename T, int MAX_SIZE>
bool TStack<T, MAX_SIZE>::IsFull() const {
    return topIndex == MAX_SIZE - 1;
}