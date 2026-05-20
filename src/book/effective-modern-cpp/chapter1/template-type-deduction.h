#ifndef TEMPLATE_TYPE_DEDUCTION_H
#define TEMPLATE_TYPE_DEDUCTION_H

// template<typename T>
// void f(ParamType param);

template <typename T>
void f1(const T& param) 
{
}

void test1()
{
    int x = 0;

    // T is deduced to be int, but ParamType is deduced to be const int&.
    f1(x);
}

//----------------------------------------------------------------------------
// Case 1: ParamType is a Reference or Pointer, but not a Universal Reference

template <typename T>
void f2(T& param)
{
}

void test2()
{
    // T is int, param's type is int&
    int x = 27;
    f2(x);

    // Note: the constness of the object becomes part of the type deduced for T

    // T is const int, param's type is const int&
    const int cx = x;
    f2(cx);

    // Note: rx’s reference-ness is ignored during type deduction

    // T is const int, param's type is const int&
    const int& rx = x;
    f2(rx);
}

#endif
