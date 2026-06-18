#pragma once

#include<string>

class Employee
{
    public:
        Employee(const std::string & n, double s): name{n}, salary {s} {}

        const std::string & getName() const
        {
            return name;
        }

        bool operator==(const Employee & rhs) const
        {
            return getName() == rhs.getName();
        }

        bool operator!=(const Employee & rhs) const
        {
            return !(*this == rhs);
        }

    private:
        std::string name;
        double salary;
};
