#ifndef ARRAY_STACK_H
#define ARRAY_STACK_H

#include <stdexcept>
#include <algorithm>

/*
 *  int main(int argc, char *argv[]) 
 *  {
 *      ArrayStack<int> a;
 *  
 *      cout << "Current size: " << a.size() << endl;
 *      cout << "Is empty? " << (a.empty() ? 'Y':'N') << endl;
 *     
 *      a.push(47);
 *      a.push(19);
 *      a.push(73);
 *  
 *      cout << "`a` new size: " << a.size() << endl;
 *      cout << "Is empty? " << (a.empty() ? 'Y':'N') << endl;
 *      
 *      for (; !a.empty();) {
 *          cout << "`a` element: " << a.pop() << endl;
 *      }
 *  
 *      return 0;
 *  }
 */
template <typename Object>
class ArrayStack
{
    public:
        /*
         * constexpr (short for constant expression) tells the compiler:
         *
         *   "If you have enough information at compile time to calculate this value or run this function, do it right now so the running program 
         *    doesn't have to do it later."
         *
         */
        static constexpr int DEFAULT_SIZE = 2;

        /*
         * The Rule of Three states that if your class manages a raw resource (like a raw heap pointer allocated via new), you almost 
         * certainly need to explicitly define three special member functions:
         * 
         *   Destructor (to free the memory)
         *   Copy Constructor (to duplicate the memory)
         *   Copy Assignment Operator (to clean up existing memory and duplicate new memory)
         * 
         */
        ArrayStack(): theArray {new Object[DEFAULT_SIZE]}, theArraySize {DEFAULT_SIZE}, topOfStack {-1}
        {
        }

        // Explicitly block shallow-copy duplication to prevent pointer corruption / double-free crashes
        ArrayStack(const ArrayStack & rhs) = delete;
        ArrayStack & operator=(const ArrayStack & rhs) = delete;

        // In C++, the moment you explicitly modify or delete your copy constructor or copy assignment operator, the compiler automatically 
        // suppresses the generation of the default move constructor and move assignment operator.

        ~ArrayStack()
        {
            delete[] theArray;
        }

        int size() const
        {
            return (topOfStack + 1);
        }

        bool empty() const
        {
            return topOfStack == -1;
        }

        void push(const Object & x)
        {
            if (topOfStack >= theArraySize - 1) {
                resize();
            }

            theArray[++topOfStack] = x;
        }

        void push(Object && x)
        {
            if (topOfStack >= theArraySize - 1) {
                resize();
            }

            theArray[++topOfStack] = std::move(x);
        }

        Object pop()
        {
            if (empty()) {
                throw std::underflow_error("Cannot pop from an empty stack");
            }

            return std::move(theArray[topOfStack--]);
        }

    private:
        Object* theArray;
        int theArraySize;
        int topOfStack;

        void resize() 
        {
            int newSize = theArraySize * 2;
            Object * newArray = new Object[newSize];
            
            for (int i = 0; i < theArraySize; i++) {
                newArray[i] = std::move(theArray[i]);
            }
            std::swap(theArray, newArray);

            delete[] newArray;
            theArraySize = newSize;
        }
};

#endif
