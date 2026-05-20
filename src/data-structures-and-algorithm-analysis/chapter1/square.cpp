#include "square.h"

double Square::getSide() const
{
    return side;
}

double Square::getArea() const
{
    return side * side;
}

double Square::getPerimeter() const
{
    return side * 4;
}

void Square::print(ostream & out) const
{
    out << "(square " << getSide() << ")";
}

// Note that we remove the Square:: prefix because it is no longer a member of the class.
ostream& operator<<(std::ostream& out, const Square& s)
{
    out << "(square " << s.getSide() << ")";
    return out;
}

bool Square::operator< (const Square & rhs) const
{
    return getSide() < rhs.side;
}
