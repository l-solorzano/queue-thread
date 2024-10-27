/// This class allows to queue a finite number of primitive types

#include <condition_variable>
#include <mutex>
#include <thread>

template <class T>
class Queue 
{
public:
    /**
     * @brief Construct a new Queue object
     * 
     * @param size The size of the Queue
     */
    Queue(int size)
    : queue_elements(new T[size])
    , count(0)
    , size(size)
    {}

    /**
     * @brief Destroy the Queue object
     * 
     */
    ~Queue()
    {
        delete[] queue_elements;
    }
    
    /**
     * @brief Push an element at the queue
     * 
     * @param element The element to push
     */
    void Push(T element)
    {
        std::unique_lock<std::mutex> lock(mutex);

        // Drop the oldest element if the queue is full
        if (count == size)
        {
            for (int i = 1; i < count; i++)
            {
                queue_elements[i - 1] = queue_elements[i];
            }

            count--;
        }

        queue_elements[count] = element;
        count++;

        queue_not_empty.notify_one();
    }

    /**
     * @brief Retrieves the element at the front of the queue
     * 
     * This method will block indefinitely if there are no elements in the queue
     * 
     * @return The element at the front of the queue
     */
    T Pop()
    {
        std::unique_lock<std::mutex> lock(mutex);

        // Wait until the queue is not empty
        queue_not_empty.wait(lock, [this] {return count > 0;});

        // Pop the oldest element
        T pop_element = queue_elements[0];

        for (int i = 1; i < count; i++)
        {
            queue_elements[i-1] = queue_elements[i];
        }

        count--;

        return pop_element;
    }

    /**
     * @brief Retrieves the element at the front of the queue with a timeout condition
     * 
     * This method will throw an exception after a given timeout if no element is pushed meanwhile
     * 
     * @param milliseconds The timeout in milliseconds
     * @return The element at the front of the queue
     * @throw std::runtime_error if no element is pushed within the timeout
     */
    T PopWithTimeout(int milliseconds)
    {
        std::unique_lock<std::mutex> lock(mutex);

        // Wait for the queue to be non-empty with a timeout
        if (!queue_not_empty.wait_for(lock, std::chrono::microseconds(milliseconds), [this] {return count > 0;}))
        {
            throw::std::runtime_error("Timeout: not element has been pushed");
        }

        // Pop the oldest element
        T pop_element = queue_elements[0];

        for (int i = 1; i < count; i++)
        {
            queue_elements[i-1] = queue_elements[i];
        }

        count--;

        return pop_element;
        
    }

    /**
     * @brief Gets the number of elements stored in the queue
     * 
     * @return The current number of elements in the queue
     */
    int Count()
    {
        std::lock_guard<std::mutex> lock(mutex);

        return count;
    }

    /**
     * @brief Gets the size of the queue
     * 
     * @return The size of the queue
     */
    int Size() const
    {
        return size;
    }

    /**
     * @brief Print the Queue elements
     * 
     */
    void PrintQueue()
    {
        std::lock_guard<std::mutex> lock(mutex);

        if (count == 0)
        {
            std::cout << "Queue is empty" << std::endl;
            return;
        }

        std::cout << "[";
        for (int i = 0; i < count; i++)
        {
            std::cout << queue_elements[i];

            if (i < count - 1)
            {
                std::cout << ",";
            }
        }
        std::cout << "]" << std::endl;
    }

private:
    /// The queue elements
    T* queue_elements;

    /// Amount of elements stored 
    int count;

    /// Size of the queue
    const int size;

    /// A mutex to protect the writing and reading threads
    std::mutex mutex;

    /// Condition variable to notify the threads that the queue is not empty
    std::condition_variable queue_not_empty;
};