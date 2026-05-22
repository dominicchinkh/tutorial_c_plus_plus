#ifndef VECTOR_STACK_H
#define VECTOR_STACK_H

#include <stdexcept>
#include <vector>

template <typename Object>
class VectorStack
{
    public:
        VectorStack() {}

        ~VectorStack() {}

        int size() const
        {
            return theArray.size();
        }

        bool empty() const
        {
            return theArray.empty();
        }

        void push(const Object & x)
        {
            theArray.push_back(x);
        }

        void push(Object && x)
        {
            theArray.push_back(std::move(x));
        }

        Object pop()
        {
            // 1. Safety Guard: Check if empty first!
            if (empty()) {
                throw std::underflow_error("Cannot pop from an empty stack");
            }

            Object retVal = std::move(theArray.back());
            theArray.pop_back();
            return retVal;
        }

    private:
        std::vector<Object> theArray;
};

#endif
