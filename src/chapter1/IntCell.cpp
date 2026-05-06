#include "IntCell.h"

// Note that default parameters are specified in the interface only.
IntCell::IntCell(int initialValue): value{ initialValue } 
{
}

// Note that whether a member function is an accessor (via the `const` at the end) or a mutator 
// is part of the signature
int IntCell::read() const 
{ 
    return value; 
}

void IntCell::write(int x) 
{ 
    value = x; 
}
