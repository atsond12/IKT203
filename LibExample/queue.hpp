#pragma once
#include <stdexcept> // For std::runtime_error

template <typename T, int MAX_SIZE>
class TQueue {
    private:
    T data[MAX_SIZE]; // A static array of T with MAX_SIZE elements
    int frontIndex;   // The index of the front element
    int rearIndex;    // The index of the rear element
	int count;        // Current number of elements in the queue
    public:
    // Constructor
    TQueue();
    // No destructor is needed since we aren't using 'new'
    // --- Core Queue Operations ---
    void Enqueue(const T& aElement);
    T Dequeue();
    T GetFront() const;
    // --- Helper Functions ---
    int size() const;
    bool IsEmpty() const;
	bool IsFull() const;
};
// --- Method Implementations ---
// Constructor: Initializes the front and rear indices
template <typename T, int MAX_SIZE>
TQueue<T, MAX_SIZE>::TQueue() : frontIndex(0), rearIndex(-1), count(0) {
    // No memory allocation is needed
}
// enqueue(): Adds an element to the rear of the queue
template <typename T, int MAX_SIZE>
void TQueue<T, MAX_SIZE>::Enqueue(const T& aElement) {
    if (IsFull()) {
        throw std::runtime_error("Queue overflow: cannot enqueue to a full queue.");
    }
    rearIndex = (rearIndex + 1) % MAX_SIZE; // Circular increment
    data[rearIndex] = aElement;
    count++;
}
// dequeue(): Removes and returns the front element
template <typename T, int MAX_SIZE>
T TQueue<T, MAX_SIZE>::Dequeue() {
    if (IsEmpty()) {
        throw std::runtime_error("Queue underflow: cannot dequeue from an empty queue.");
    }
    T frontElement = data[frontIndex];
    frontIndex = (frontIndex + 1) % MAX_SIZE; // Circular increment
    count--;
    return frontElement;
}
// getFront(): Returns the front element without removing it
template <typename T, int MAX_SIZE>
T TQueue<T, MAX_SIZE>::GetFront() const {
    if (IsEmpty()) {
        throw std::runtime_error("Queue is empty: cannot get front.");
    }
    return data[frontIndex];
}
// size(): Returns the current number of elements
template <typename T, int MAX_SIZE>
int TQueue<T, MAX_SIZE>::size() const {
    return count;
}
// isEmpty(): Checks if the queue is empty
template <typename T, int MAX_SIZE>
bool TQueue<T, MAX_SIZE>::IsEmpty() const {
    return count == 0;
}
// isFull(): Checks if the queue is full
template <typename T, int MAX_SIZE>
bool TQueue<T, MAX_SIZE>::IsFull() const {
    return count == MAX_SIZE;
}