#include <gtest/gtest.h>

#include "Queue.hpp"

/**
 * @brief Test the Count and Size methods of the Queue class
 * 
 */
TEST(queue_test, CountAndSize)
{
    Queue<char> queue(2);

    queue.Push('a');

    // Size should be 2 and count 1
    EXPECT_EQ(queue.Size(), 2);
    EXPECT_EQ(queue.Count(), 1);
}

/**
 * @brief Test the Push and Pop methods of the Queue class
 * 
 */
TEST(queue_test, PushAndPop)
{
    Queue<int> queue(3);

    // Push four elements
    queue.Push(1);
    queue.Push(2);
    queue.Push(3);
    queue.Push(4);

    // Count should be 3, because oldest elements is dropped 
    EXPECT_EQ(queue.Count(), 3);

    // Pop all the elements
    queue.Pop();
    queue.Pop();
    queue.Pop();
    EXPECT_EQ(queue.Count(), 0);
}

/**
 * @brief Test the PopWithTimeout method of the Queue class
 * 
 */
TEST(queue_test, PopWithTimeout)
{
    Queue<int> queue(2);

    // Fill the queue
    queue.Push(1);
    queue.Push(2);

    // Pop all element
    queue.PopWithTimeout(50);
    queue.PopWithTimeout(50);
    EXPECT_EQ(queue.Count(), 0);

    // No more elements have been push, expect and exception after the timeout
    EXPECT_THROW(queue.PopWithTimeout(50), std::runtime_error);
}