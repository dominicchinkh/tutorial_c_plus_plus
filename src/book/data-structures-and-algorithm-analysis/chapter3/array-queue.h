#pragma once

#include <iostream>

/*
 *  int main(int argc, char *argv[]) 
 *  {
 *      ArrayQueue<int> a;
 *  
 *      a.enqueue(34);
 *      a.enqueue(65);
 *      a.enqueue(98);
 *  
 *      // a.dequeue();
 *      // a.dequeue();
 *      // a.dequeue();
 *  
 *      // std::cout << a.dequeue() << std::endl;
 *      // std::cout << a.dequeue() << std::endl;
 *      // std::cout << a.dequeue() << std::endl;
 *  
 *      // ArrayQueue copy
 *      ArrayQueue<int> b(a);
 *  
 *      std::cout << b.dequeue() << std::endl;
 *      std::cout << b.dequeue() << std::endl;
 *      std::cout << b.dequeue() << std::endl;
 *  
 *      return 0;
 *  }
 */
template <typename Object>
class ArrayQueue
{
    static constexpr int CAPACITY = 4;

    public:
        ArrayQueue(): theArray { new Object[CAPACITY] }, front {0}, back {0}, size {0} {}

        ~ArrayQueue()
        {
            delete[] theArray;
        }

        ArrayQueue(const ArrayQueue & rhs): 
            theArray { new Object[CAPACITY] }, front {rhs.front}, back {rhs.back}, size {rhs.size}
        {
            // If an element inside rhs.theArray has already been dequeued, it contains a stale or 
            // default-constructed object state. More importantly, if Object is a complex type without
            // a safe default state (or a resource-heavy type), copying every index blindly wastes 
            // performance and might copy uninitialised data

            // Loop through the valid circular indices based on the size and front position of rhs

            int current = rhs.front;

            for (int i = 0; i < rhs.size; ++i) {
                theArray[current] = rhs.theArray[current];
                current = (current + 1) % CAPACITY;
            }
        }

        // https://en.cppreference.com/cpp/language/exceptions#Exception_safety

        // To make the strong exception guarantee possible, user-defined move constructors should not 
        // throw exceptions. For example, std::vector relies on std::move_if_noexcept to choose between 
        // move and copy when the elements need to be relocated.

        ArrayQueue(ArrayQueue && rhs) noexcept: 
            theArray { rhs.theArray }, front {rhs.front}, back {rhs.back}, size {rhs.size}
        {
            rhs.theArray = nullptr;
            rhs.front = 0;
            rhs.back = 0;
            rhs.size = 0;
        }

        ArrayQueue & operator=(const ArrayQueue & rhs)
        {
            // Self-assignment guard
            if (this != &rhs) {
                ArrayQueue temp(rhs); 
                std::swap(theArray, temp.theArray);
                std::swap(front, temp.front);
                std::swap(back, temp.back);
                std::swap(size, temp.size);
            }
            return *this;
        }

        ArrayQueue & operator=(ArrayQueue && rhs) noexcept
        {
            // Self-assignment guard
            if (this != &rhs) { 
                // Clean up our own existing memory first!
                delete[] theArray; 

                // Pilfer rhs's resources
                theArray = rhs.theArray;
                front = rhs.front;
                back = rhs.back;
                size = rhs.size;

                // Reset rhs to a safe, empty state
                rhs.theArray = nullptr;
                rhs.front = 0;
                rhs.back = 0;
                rhs.size = 0;
            }
            return *this;
        }
        
        void enqueue(const Object & x) 
        {
            if (full()) {
                throw std::overflow_error("Array is full");
            }

            theArray[back] = x;
            
            back = (back + 1) % CAPACITY;
            ++size;
        }

        void enqueue(Object && x)
        {
            if (full()) {
                throw std::overflow_error("Array is full");
            }

            theArray[back] = std::move(x);

            back = (back + 1) % CAPACITY;
            ++size;
        }

        Object dequeue()
        {
            if (empty()) {
                throw std::underflow_error("Array is empty");
            }

            int currentFront = front;

            front = (front + 1) % CAPACITY;
            --size;

            return theArray[currentFront];
        }

    private:
        Object *theArray;

        int front;
        int back;
        int size;

        bool empty() const
        {
            return (size <= 0);
        }

        bool full() const
        {
            return (size >= CAPACITY);
        }
};
