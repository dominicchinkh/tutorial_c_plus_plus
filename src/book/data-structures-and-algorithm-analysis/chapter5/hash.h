#ifndef HASH_H
#define HASH_H

#include<string>

#include "employee.h"

template <typename HashedObj>
class Hash
{
    public:
        size_t operator()(const HashedObj & x) const
        {
            static Hash<HashedObj> hf;
            return hf(x);
        }    
};

template<>
class Hash<int>
{
    public:
        size_t operator()(int x)
        {
            return x;
        }
};

template <>
class Hash<std::string>
{
    public:
        // If the keys are very long, the hash function will take too long to compute.
        // A common practice in this case is not to use all the characters.

        size_t operator()(const std::string & k) const
        {
            size_t hashVal = 0;

            for (char ch: k) {
                hashVal =  37 * hashVal + ch;
            }

            return hashVal;
        }
};

template<>
class Hash<Employee>
{
    public:
        size_t operator()(const Employee & x) const
        {
            // With static: The hf object is created exactly once - the very first
            // time this function is executed. It then quietly sits in memory for 
            // the entire duration of the program's lifecycle. Subsequent calls to 
            // the function simply reuse that exact same object.

            static Hash<std::string> hf;
            return hf(x.getName());
        }
};

//-----------------------------------------------------------------------------
// If the table size is large, the function does not distribute the keys wells.

/*
int hash(const string & key, int tableSize)
{
    int hashVal = 0;

    for (char ch: key) {
        hashVal += ch;
    }

    return hashVal % tableSize;
}
*/

//----------------------------------------------------------------------------------
// Unfortunately, English is not random. Even if none of these combinations collide,
// only 28 percent of the table can actually be hashed to.

/*
int hash(const string & key, int tableSize)
{
    return (key[0] + 27 * key[1] + 729 * key[2]) % tableSize;
}
*/

#endif
