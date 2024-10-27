#include <iostream>
#include <thread>
#include <chrono>
#include "Queue.hpp"

/**
 * @brief Writing thread to push int elements to a Queue
 * 
 * @param q A reference to Queue object
 */
void writingThread(Queue<int>& q) 
{
    q.Push(1);
    q.PrintQueue();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    q.Push(2);
    q.PrintQueue();
    q.Push(3);
    q.PrintQueue();
    q.Push(4);
    q.PrintQueue();

    // Pop shall block until a new element is pushed
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    q.Push(5); 
    q.PrintQueue();
}

/**
 * @brief Reading thread to pop int elements from a Queue
 * 
 * @param q A reference to Queue object
 */
void readingThread(Queue<int>& q) 
{
    q.PopWithTimeout(80);
    q.PrintQueue();
    q.Pop();
    q.PrintQueue();
    q.Pop();
    q.PrintQueue();
    q.Pop();
    q.PrintQueue();
}

int main() {
    // Create a queue with a size of 2
    Queue<int> queue(2);

    // Create writing and reading threads
    std::thread writing(writingThread, std::ref(queue));
    std::thread reading(readingThread, std::ref(queue));

    writing.join();
    reading.join();

    return 0;
}