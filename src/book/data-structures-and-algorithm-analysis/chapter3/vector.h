#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>

/*
 *  int main(int argc, char *argv[]) 
 *  {
 *      Vector<int> a(3);
 *      a[0] = 34;
 *      a[1] = 56;
 *      a[2] = 98;
 *  
 *      Vector<int> b(a);
 *      Vector<int> c = a;
 *  
 *      cout << "b: " << b[0] << ' ' << b[1] << ' ' << b[2] << endl;
 *      cout << "c: " << c[0] << ' ' << c[1] << ' ' << c[2] << endl;
 *  
 *      cout << "Current size: " << a.size() << endl;
 *      cout << "Current capacity: " << a.capacity() << endl;
 *  
 *      a.resize(6);
 *      a.reserve(20);
 *  
 *      cout << "New size: " << a.size() << endl;
 *      cout << "New capacity: " << a.capacity() << endl;
 *      
 *      a.push_back(47);
 *      cout << "New element at index " << (a.size() - 1) << ": "  << a[a.size() - 1] << endl;
 *  
 *      a.pop_back();
 *      cout << "a new size after pop_back: " << a.size() << endl;
 *      cout << "a last element after pop_back: " << a.back() << endl;
 *  
 *      Vector<int>::iterator itr = a.begin();
 *  
 *      cout << "a elements: " << endl;
 *      for (Vector<int>::iterator itr = a.begin(); itr != a.end(); ++itr) {
 *          cout << *(itr) << endl;
 *      }
 *      
 *      return 0;
 *  }
 * 
 */
template <typename Object>
class Vector
{
    public:
        // Forward declare the nested iterator classes
        class const_iterator;
        class iterator;

        class const_iterator
        {
            public:
                const_iterator(): current {nullptr}, theVector {nullptr} {}

                const Object & operator*() const
                {
                    assertIsValid();
                    if (current == theVector->end_ptr()) {
                        throw std::out_of_range("Attempted to dereference an end() iterator");
                    }

                    return *current;
                }

                const Object * operator->() const
                {
                    assertIsValid();
                    if (current == theVector->end_ptr()) {
                        throw std::out_of_range("Attempted to access an end() iterator");
                    }

                    return current;
                }

                const_iterator operator++()
                {
                    assertIsValid();
                    if (current == theVector->end_ptr()) {
                        throw std::out_of_range("Attempted to increment past an end() iterator");
                    }

                    current++;
                    return *this;
                }

                const_iterator operator++(int)
                {
                    const_iterator old = *this;
                    ++(*this);
                    return old;
                }

                const_iterator operator--()
                {
                    assertIsValid();
                    if (current == theVector->begin_ptr()) {
                        throw std::out_of_range("Attempted to decrement past an begin() iterator");
                    }

                    current--;
                    return *this;
                }

                const_iterator operator--(int)
                {
                    const_iterator old = *this;
                    --(*this);
                    return old;
                }

                bool operator==(const const_iterator & rhs)
                {
                    return current == rhs.current;
                }

                bool operator!=(const const_iterator & rhs)
                {
                    return !(*this == rhs);
                }

            private:
                Object * current;
                const Vector<Object> * theVector;

                const_iterator(Object *p, const Vector<Object> * v):
                    current {p}, theVector {v}
                {}

                void assertIsValid() const
                {
                    if (theVector == nullptr || current == nullptr) {
                        throw std::runtime_error("Attempted to use an uninitialized iterator");
                    }
                }

                friend class Vector<Object>;
        };

        class iterator: public const_iterator
        {
            public:
                iterator() {}

                /*
                 * When a C++ compiler processes a template class, it does so in two distinct phases:
                 *
                 * Phase 1 (Syntax Check): 
                 *   The compiler looks at the template before it knows what Object is (e.g., when it 
                 *   compiles the general Vector code). It looks for variables and functions that don't
                 *   depend on the template parameters.
                 * 
                 * Phase 2 (Instantiation): 
                 *   The compiler looks at the code again when you actually create an object (like 
                 *   Vector<int>), substituting Object with int.
                 * 
                 * During Phase 1, the compiler looks at the iterator class and sees assertIsValid() 
                 * and current.
                 * 
                 * By default, C++ does not look inside dependent base classes during Phase 1. Because 
                 * the compiler doesn't know what const_iterator looks like yet, it assumes assertIsValid
                 * and current do not exist in the current scope. It will throw a compilation error: "use 
                 * of undeclared identifier" or "identifier not found".
                 * 
                 * Using this-> turns those variables and functions into dependent names, forcing the 
                 * compiler to delay its search until it fully resolves the base class const_iterator.
                 * 
                 */

                Object & operator*()
                {
                    this->assertIsValid();
                    if (this->current == this->theVector->end_ptr()) {
                        throw std::out_of_range("Attempted to dereference an end() iterator.");
                    }

                    return *(this->current);
                }

                /*
                 * C++ non-virtual function hiding.
                 *
                 * When the mutable iterator class overrides Object & operator*(), it creates a function 
                 * with the name operator*. In C++, if a derived class defines a function with the same 
                 * name as a function in the base class, it completely hides all versions of that function
                 * in the base class - even if their signatures (like const qualifiers) are completely 
                 * different.
                 * 
                 * If you try to use a const iterator in your code like this:
                 * 
                 *     void printFirst(const Vector<int>::iterator & itr) {
                 *         std::cout << *itr << std::endl; // ERROR! Compiler fails here.
                 *     }
                 * 
                 * The compiler looks at itr, sees it is a const iterator, and looks for a const version 
                 * of operator* inside the iterator class. It cannot find one there, and because the 
                 * non-const Object & operator*() hid the base class, it won't check const_iterator either.
                 * 
                 * Your compilation will fail with an error like:
                 *   error: passing 'const Vector<int>::iterator' as 'this' argument discards qualifiers
                 * 
                 * You can achieve the exact same result using a using declaration.
                 * 
                 *     using const_iterator::operator*;
                 * 
                 */
                const Object & operator*() const
                {
                    return const_iterator::operator*();
                }

                Object * operator->()
                {
                    this->assertIsValid();
                    if (this->current == this->theVector->end_ptr()) {
                        throw std::out_of_range("Attempted to access member of an end() iterator.");
                    }

                    return this->current;
                }

                const Object * operator->() const
                {
                    return const_iterator::operator->();
                }

                iterator operator++()
                {
                    const_iterator::operator++();
                    return *this;
                }

                iterator operator++(int)
                {
                    iterator old = *this;
                    const_iterator::operator++();
                    return old;
                }

                iterator operator--()
                {
                    const_iterator::operator--();
                    return *this;
                }

                iterator operator--(int)
                {
                    iterator old = *this;
                    const_iterator::operator--();
                    return old;
                }

            private:
                iterator(Object *p, const Vector<Object> * v): const_iterator {p, v} {}

                friend class Vector<Object>;

        };

        static const int SPARE_CAPACITY = 16;

        explicit Vector(int size = 0): theSize {size}, theCapacity {size + SPARE_CAPACITY}
        {
            objects = new Object[theCapacity];
        }

        Vector(const Vector & rhs): 
            theSize {rhs.theSize}, theCapacity {rhs.theCapacity}, objects {nullptr}
        {
            objects = new Object[theCapacity];
            for (int i = 0; i < theSize; i++) {
                objects[i] = rhs.objects[i];
            }
        }

        Vector(Vector && rhs): theSize {rhs.theSize}, theCapacity {rhs.theCapacity}, objects {rhs.objects}
        {
            rhs.objects = nullptr;
            rhs.theSize = 0;
            rhs.theCapacity = 0;
        }

        ~Vector() 
        {
            delete[] objects;
        }

        Vector & operator=(const Vector & rhs)
        {
            // In the case where both Vectors have the same size, which can be tested for, it can be 
            // more efficient to simply copy each element one by one using Object’s operator=

            Vector copy = rhs;
            std::swap(*this, copy);
            return *this;
        }
        
        Vector & operator=(Vector && rhs)
        {
            // Swapping ensures that rhs gets a perfectly matched set of parameters (your old size, old 
            // capacity, and old pointer), keeping it completely stable for its impending destruction.

            std::swap(theSize, rhs.theSize);
            std::swap(theCapacity, rhs.theCapacity);
            std::swap(objects, rhs.objects);

            return *this;
        }

        void resize(int newSize)
        {
            if (newSize > theCapacity) {
                reserve(newSize * 2);
            }
            theSize = newSize;
        }

        void reserve (int newCapacity)
        {
            if (newCapacity < theSize) {
                return;
            }

            Object *newArray = new Object[newCapacity];
            for (int i = 0; i < theSize; ++i) {
                newArray[i] = std::move(objects[i]);
            }

            theCapacity = newCapacity;

            std::swap(objects, newArray);
            delete[] newArray;
        }

        Object & operator[](int index)
        {
            if (index < 0 || index >= theSize) {
                throw std::out_of_range("Index is out of range");
            }

            return objects[index];
        }

        const Object & operator[](int index) const
        {
            if (index < 0 || index >= theSize) {
                throw std::out_of_range("Index is out of range");
            }

            return objects[index];
        }

        bool empty() const
        {
            return size() == 0;
        }

        int size() const
        {
            return theSize;
        }

        int capacity() const
        {
            return theCapacity;
        }

        void push_back(const Object & x)
        {
            if (theSize == theCapacity) {
                reserve(2 * theCapacity + 1);
            }
            objects[theSize++] = x;
        }

        void push_back(Object && x)
        {
            if (theSize == theCapacity) {
                reserve(2 * theCapacity + 1);
            }
            objects[theSize++] = std::move(x);
        }

        void pop_back()
        {
            // Error checks in which an exception is thrown if the size is 0
            --theSize;
        }

        const Object & back()
        {
            // Error checks in which an exception is thrown if the size is 0
            return objects[theSize - 1];
        }

        iterator begin()
        {
            return iterator(&objects[0], this);
        }

        const_iterator begin() const
        {
            return const_iterator(&objects[0], this);
        }

        iterator end()
        {
            return iterator(&objects[size()], this);
        }

        const_iterator end() const
        {
            return const_iterator(&objects[size()], this);
        }

    private:
        int theSize;
        int theCapacity;
        Object * objects;

        Object * begin_ptr() const
        {
            return &objects[0];
        }

        Object * end_ptr() const
        {
            return &objects[theSize];
        }
};

#endif
