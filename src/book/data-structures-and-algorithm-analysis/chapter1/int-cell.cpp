#include <utility>
#include "int-cell.h"

// Note that default parameters are specified in the interface only.
IntCell::IntCell(int initialValue) 
{ 
    value = new int {initialValue}; 
} 

IntCell::~IntCell()
{
    delete value;
}

IntCell::IntCell(const IntCell & rhs)
{
    value = new int{*rhs.value};
}

// Note that if there is non-primitive data, then that data must be moved in the initialization list
IntCell::IntCell(IntCell && rhs): value { rhs.value }
{
    rhs.value = nullptr;
}

IntCell & IntCell::operator=(const IntCell & rhs)
{
    if (this != &rhs) {
        // Member-by-member swap
        *value = *rhs.value;
    }
    return *this;
}

// In C++11, copy assignment is often written using a copy-and-swap idiom
/*
IntCell & IntCell::operator=(const IntCell & rhs)
{
    IntCell copy = rhs;
    swap(*this, copy);
    return *this;
}
*/

IntCell & IntCell::operator=(IntCell && rhs)
{
    swap(value, rhs.value);
    return *this;
}

// Note that whether a member function is an accessor (via the `const` at the end) or a mutator 
// is part of the signature
int IntCell::read() const 
{ 
    return *value; 
}

void IntCell::write(int x) 
{ 
    *value = x; 
}
