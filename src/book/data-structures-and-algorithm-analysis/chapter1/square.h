#pragma once

#include <iostream>

using namespace std;

class Square
{
    public:
        explicit Square(double s = 0.0): side {s} {}

        double getSide() const;
        double getArea() const;
        double getPerimeter() const;

        void print(ostream & out = cout) const;

        // In C++, the insertion operator (<<) must be implemented as a non-member function 
        // (usually a friend) if you want to use it with the standard syntax: cout << mySquare;
        friend ostream& operator<<(ostream& out, const Square& s);

        bool operator< (const Square & rhs) const;

    private:
        double side;
};
