#pragma once

#include <string>

#include "uniform-random.h"

// template <typename AnyType>
// class CuckooHashFamily
// {
//     public:
//         size_t hash(const AnyType & x, int which) const;
//         int getNumberOfFunctions();
//         void generateNewFunctions();
// };

template <int count>
class StringHashFamily
{
    public:
        StringHashFamily(): MULTIPLIERS(count)
        {
            generateNewFunctions();
        }

        int getNumberOfFunctions() const
        {
            return count;
        }

        void generateNewFunctions()
        {
            for (auto & mult: MULTIPLIERS) {
                mult = r.nextInt();
            }
        }

        size_t hash(const std::string & x, int which) const
        {
            const int multiplier = MULTIPLIERS[which];

            size_t hashVal = 0;
            for (auto ch: x) {
                hashVal = multiplier * hashVal + ch;
            }

            return hashVal;
        }

    private:
        std::vector<int> MULTIPLIERS;
        UniformRandom r;
};
