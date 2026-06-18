#pragma once

#include <iostream>

using namespace std;

/*
 * C++11 allows one to write code that works even if the Container type does not have begin and end 
 * member functions. 
 * 
 * Non-member free functions begin and end are defined that allow one to use begin(c) in any place 
 * where c.begin() is allowed. 
 * 
 * Writing generic code using begin(c) instead of c.begin() has the advantage that it allows the 
 * generic code to work on containers that have begin/end as members, as well as those that do not 
 * have begin/end but which can later be augmented with appropriate non-member functions.
 * 
 */

/*  void main()
 *  { 
 *    vector<int> a = {4, 7, 9};
 *    print(a);
 *  }
 */

/*-------------------------------------------------------------------------
 * error: call of overloaded ‘begin(const std::vector<int>&)’ is ambiguous

 * Even though you didn't write std::begin(v), the compiler checks the std namespace anyway because 
 * v is a std::vector. Your template and std::begin have virtually the exact same signature, so the 
 * compiler cannot decide which one you intended to use.
 * 
 * If you want to use your version of begin, wrap your templates in a custom namespace and call it 
 * explicitly. This completely bypasses the ADL ambiguity.
 * 
 */
namespace container {
    
    //---------------
    // Before C++ 14

    /*
     *  decltype is telling the compiler, "Look at whatever type c.begin() returns, and make that
     *  the return type of this function."
     *
     *  It stands for declared type. It acts as a compile-time operator that inspects the type of 
     *  an expression without actually executing any code.
     * 
     */

    // template <typename Container>
    // auto begin(Container & c) -> decltype(c.begin())
    // {
    //     return c.begin();
    // }

    // template <typename Container>
    // auto begin(const Container & c) -> decltype(c.begin())
    // {
    //     return c.begin();
    // }

    //--------------
    // After C++ 14
    
    template <typename Container>
    auto begin(Container & c)
    {
        return c.begin();
    }

    template <typename Container>
    auto begin(const Container & c)
    {
        return c.begin();
    }
}

template <typename Container>
void print(const Container & c, ostream & out = cout)
{
    if (c.empty()) {
        out << "(empty)";
    }
    else {
        // itr is a Container::const_iterator
        auto itr = container::begin(c); 

        // Print first itemm
        out << "[ " << *itr++;

        while (itr != end(c)) {
            out << ", " << *itr++;
        }
        out << " ]" << endl;
    }
}
