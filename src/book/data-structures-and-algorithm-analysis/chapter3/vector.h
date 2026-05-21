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
        static const int SPARE_CAPACITY = 16;

        explicit Vector(int size = 0): theSize {size}, theCapacity {size + SPARE_CAPACITY}
        {
            objects = new Object[theCapacity];
        }

        Vector(const Vector & rhs): theSize {rhs.theSize}, theCapacity {rhs.theCapacity}, objects {nullptr}
        {
            objects = new Object[theCapacity];
            for (int i = 0; i < theSize; i++) {
                objects[i] = rhs.objects[i];
            }
        }

        Vector & operator=(const Vector & rhs)
        {
            // In the case where both Vectors have the same size, which can be tested for, it can be 
            // more efficient to simply copy each element one by one using Object’s operator=

            Vector copy = rhs;
            std::swap(*this, copy);
            return *this;
        }

        ~Vector() 
        {
            delete[] objects;
        }

        Vector(Vector && rhs): theSize {rhs.theSize}, theCapacity {rhs.theCapacity}, objects {rhs.objects}
        {
            rhs.objects = nullptr;
            rhs.theSize = 0;
            rhs.theCapacity = 0;
        }

        Vector & operator=(Vector && rhs)
        {
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
            for (int i = 0; i < theSize; i++) {
                newArray[i] = std::move(objects[i]);
            }

            theCapacity = newCapacity;

            std::swap(objects, newArray);
            delete[] newArray;
        }

        Object & operator[](int index)
        {
            // Add error checking by making sure that index is in the range 0 to size()-1
            return objects[index];
        }

        const Object & operator[](int index) const
        {
            // Add error checking by making sure that index is in the range 0 to size()-1
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

        // If the iterator itr goes crashing past the end marker, neither ++itr nor *itr will 
        // necessarily signal an error. To fix this problem would require that the iterator and 
        // const_iterator be actual nested class types rather than simply pointer variables.

        typedef Object * iterator;
        typedef const Object * const_iterator;

        iterator begin()
        {
            return &objects[0];
        }

        const_iterator begin() const
        {
            return &objects[0];
        }

        iterator end()
        {
            return &objects[size()];
        }

        const_iterator end() const
        {
            return &objects[size()];
        }

    private:
        int theSize;
        int theCapacity;
        Object * objects;
};

#endif
