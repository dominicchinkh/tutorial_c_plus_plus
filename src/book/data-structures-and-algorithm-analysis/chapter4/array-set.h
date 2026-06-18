#pragma once

#include <algorithm>
#include <iostream>
#include <optional>

/*
 *  int main(int argc, char *argv[]) 
 *  {
 *      ArraySet<int> a(3);
 *      a[0] = 34;
 *      a[1] = 56;
 *      a[2] = 98;
 *  
 *      ArraySet<int> b(a);
 *      ArraySet<int> c = a;
 *  
 *      cout << "b: " << b[0] << ' ' << b[1] << ' ' << b[2] << endl;
 *      cout << "c: " << c[0] << ' ' << c[1] << ' ' << c[2] << endl;
 *  
 *      cout << "Current size: " << a.size() << endl;
 *      cout << "Current capacity: " << a.capacity() << endl;
 *  
 *      a.insert(47);
 *      cout << "New element at index " << (a.size() - 1) << ": "  << a[a.size() - 1] << endl;
 *  
 *      a.print();
 *  
 *      a.erase(56);
 *      cout << "a new size after erase: " << a.size() << endl;
 *  
 *      a.print();
 *  
 *      a.reserve(20);
 *  
 *      cout << "New size after reserve: " << a.size() << endl;
 *      cout << "New capacity after reserve: " << a.capacity() << endl;
 *  
 *      auto itr = a.begin();
 *  
 *      cout << "a elements: " << endl;
 *      for (auto itr = a.begin(); itr != a.end(); ++itr) {
 *          cout << *(itr) << endl;
 *      }
 *  
 *      a.print();
 *  
 *      auto result = a.find(47);
 *      if (result) {
 *          // Use the dereference operator (*) on the optional to get the iterator,
 *          // then dereference the iterator to get the value.
 *          cout << **(result) << endl;
 *      }
 *      
 *      return 0;
 *  }
 */
template <typename Object>
class ArraySet
{
    public:
        // Forward declare the nested iterator classes
        class const_iterator;
        class iterator;

        class const_iterator
        {
            public:
                // Add these lines so std::find works:
                using iterator_category = std::bidirectional_iterator_tag;
                using value_type        = Object;
                using difference_type   = std::ptrdiff_t;
                using pointer           = const Object*;
                using reference         = const Object&;

                const_iterator(): current {nullptr}, theSet {nullptr} {}

                const Object & operator*() const
                {
                    assertIsValid();
                    if (current == theSet->end_ptr()) {
                        throw std::out_of_range("Attempted to dereference an end() iterator");
                    }

                    return *current;
                }

                // The arrow operator -> in C++ has a very specific rule: it must return either 
                // a raw pointer or another object that itself overloads ->
                const Object * operator->() const
                {
                    assertIsValid();
                    if (current == theSet->end_ptr()) {
                        throw std::out_of_range("Attempted to dereference an end() iterator");
                    }

                    return current;
                }

                const_iterator & operator++()
                {
                    assertIsValid();
                    if (current == theSet->end_ptr()) {
                        throw std::out_of_range("Attempted to increment pass end() iterator");
                    }

                    ++current;
                    return *this;
                }

                const_iterator operator++(int)
                {
                    const_iterator old = *this;
                    ++(*this);
                    return old;
                }

                const_iterator & operator--()
                {
                    assertIsValid();
                    if (current == theSet->begin_ptr()) {
                        throw std::out_of_range("Attempted to decrement past an begin() iterator");
                    }

                    --current;
                    return *this;
                }

                const_iterator operator--(int)
                {
                    const_iterator old = *this;
                    --(*this);
                    return old;
                }

                bool operator==(const const_iterator & rhs) const
                {
                    return current == rhs.current;
                }

                bool operator!=(const const_iterator & rhs) const
                {
                    return !(*this == rhs);
                }

            private:
                // Can modify current itself (meaning you can change what it points to), but you cannot modify the 
                // data inside the object it points to.
                const Object * current;
                const ArraySet<Object> * theSet;

                const_iterator(const Object * p, const ArraySet<Object> * s): current {p}, theSet {s} {}

                void assertIsValid() const
                {
                    if (theSet == nullptr || current == nullptr) {
                        throw std::runtime_error("Attempted to use an uninitialized iterator");
                    }
                }

                friend class ArraySet<Object>;
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
                 *   compiles the general ArraySet code). It looks for variables and functions that don't
                 *   depend on the template parameters.
                 * 
                 * Phase 2 (Instantiation): 
                 *   The compiler looks at the code again when you actually create an object (like 
                 *   ArraySet<int>), substituting Object with int.
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
                    if (this->current == this->theSet->end_ptr()) {
                        throw std::out_of_range("Attempted to dereference an end() iterator.");
                    }

                    return const_cast<Object &>(*(this->current));
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
                 *     void printFirst(const ArraySet<int>::iterator & itr) {
                 *         std::cout << *itr << std::endl; // ERROR! Compiler fails here.
                 *     }
                 * 
                 * The compiler looks at itr, sees it is a const iterator, and looks for a const version 
                 * of operator* inside the iterator class. It cannot find one there, and because the 
                 * non-const Object & operator*() hid the base class, it won't check const_iterator either.
                 * 
                 * Your compilation will fail with an error like:
                 *   error: passing 'const ArraySet<int>::iterator' as 'this' argument discards qualifiers
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
                    if (this->current == this->theSet->end_ptr()) {
                        throw std::out_of_range("Attempted to access member of an end() iterator.");
                    }

                    // `current` is `const Object*` in the base class, iterator::operator->() will fail
                    // to compile here because it tries to cast away const-ness implicitly to return a
                    // mutable Object*. To fix this, you must use a const_cast

                    return const_cast<Object*>(this->current);
                }

                const Object * operator->() const
                {
                    return const_iterator::operator->();
                }

                iterator & operator++()
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

                iterator & operator--()
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
                iterator(Object * p, const ArraySet<Object> * s): const_iterator {p, s} {}

                friend class ArraySet<Object>;
        };

        static const int SPARE_CAPACITY = 16;

        explicit ArraySet(int size = 0): theSize {size}, theCapacity {size + SPARE_CAPACITY}
        {
            objects = new Object[theCapacity];
        }

        ArraySet(const ArraySet & rhs): 
            theSize {rhs.theSize}, theCapacity {rhs.theCapacity}, objects {nullptr}
        {
            objects = new Object[theCapacity];
            for (int i = 0; i < theSize; ++i) {
                objects[i] = rhs.objects[i];
            }
        }

        ArraySet(ArraySet && rhs): 
            theSize {rhs.theSize}, theCapacity {rhs.theCapacity}, objects {std::move(rhs.objects)}
        {
            rhs.objects = nullptr;
            rhs.theSize = 0;
            rhs.theCapacity = 0;
        }

        ~ArraySet()
        {
            delete[] objects;
        }

        ArraySet & operator=(const ArraySet & rhs)
        {
            // In the case where both Sets have the same size, which can be tested for, it can be 
            // more efficient to simply copy each element one by one using Object’s operator=

            ArraySet *copy = rhs;
            std::swap(*this, copy);
            return *this;
        }

        ArraySet & operator=(ArraySet && rhs)
        {
            std::swap(theSize, rhs.theSize);
            std::swap(theCapacity, rhs.theCapacity);
            std::swap(objects, rhs.objects);

            return *this;
        }

        void reserve(int newCapacity)
        {
            if (newCapacity < theSize) {
                return;
            }

            Object* newObjects = new Object[newCapacity];
            for (int i = 0; i < theSize; ++i) {
                newObjects[i] = std::move(objects[i]);
            }

            theCapacity = newCapacity;

            std::swap(objects, newObjects);
            delete[] newObjects;
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
            return theSize == 0;
        }

        int size() const
        {
            return theSize;
        }

        int capacity() const
        {
            return theCapacity;
        }

        std::optional<const_iterator> find(const Object & x) const
        {
            auto itr = std::find(begin(), end(), x);
            if (itr != end()) {
                return itr;
            }

            return std::nullopt;
        }

        std::optional<iterator> insert(const Object & x) 
        {
            auto itr = std::find(begin(), end(), x);

            if (itr != end()) {
                return std::nullopt;
            }

            if (theSize == theCapacity) {
                reserve(theCapacity * 2);
            }
            
            objects[theSize] = x;
            ++theSize;

            // Returns an iterator that represents where x is
            return iterator(&objects[theSize - 1], this);
        }

        int erase(const Object & x)
        {
            auto itr = std::find(begin(), end(), x);
            if (itr != end()) {
                auto result = erase(itr);

                if (result) {
                    return 1; // Successful
                }
            }

            return 0; // Not successful
        }

        std::optional<iterator> erase(iterator itr)
        {
            if (itr == end()) {
                return std::nullopt;
            }

            size_t index = &(*itr) - begin_ptr();
            size_t lastIndex = theSize - 1;

            // If it's not already the last element, swap it with the last element
            if (index != lastIndex) {
                objects[index] = std::move(objects[lastIndex]);
            }

            --theSize;

            // By standard convention, erase returns an iterator to the element 
            // now occupying the position of the erased element.

            // If we erased the last element, index == theSize, which is end().
            return iterator(&objects[index], this);
        }

        std::optional<iterator> erase(iterator start, iterator end)
        {
            if (start == end) {
                return std::nullopt;
            }

            size_t startIndex = &(*start) - begin_ptr();
            size_t endIndex   = &(*end) - begin_ptr();
            size_t numToRemove = endIndex - startIndex;

            for (int i = 0; i < numToRemove; ++i) {
                size_t targetIndex = startIndex + i;
                size_t sourceIndex = theSize - 1 - i;

                // If the source element is outside of our deletion zone, move it in
                if (sourceIndex >= endIndex) {
                    objects[targetIndex] = std::move(objects[sourceIndex]);
                }
            }

            theSize -= numToRemove;

            // Return the iterator to the element now occupying the start position
            return iterator(&objects[startIndex], this);
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
            return iterator(&objects[theSize], this);
        }

        const_iterator end() const
        {
            return const_iterator(&objects[theSize], this);
        }

        void print(std::ostream & out = std::cout) const
        {
            if (empty()) {
                out << "(empty)";
            }
            else {
                auto itr = begin();

                out << "[ " << *itr++;

                while (itr != end()) {
                    out << ", " << *itr++;
                }

                out << " ]" << std::endl;
            }
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
