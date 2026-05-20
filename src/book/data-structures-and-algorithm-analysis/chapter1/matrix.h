#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

using namespace std;

template <typename Object>
class Matrix
{
    public:
        Matrix(vector<vector<Object>> v): array {v} {}
        Matrix(vector<vector<Object>> && v): array { move(v) } {}

        Matrix(int row, int column): array { row } 
        {
            for (auto & thisRow: array) {
                thisRow.resize(column);
            }
        }

        /*
         *   to[ i ] = from[ i ];
         *
         * We need operator[] to return a constant reference for `from`, but a plain 
         * reference for `to`.
         *
         * If operator[] returns a constant reference, then to[i] cannot appear on the 
         * left side of the assignment statement. Thus, operator[] should return
         * a reference.
         * 
         * Since member function const-ness (i.e., whether a function is an accessor or a 
         * mutator) is part of the signature, we can have the accessor version of operator[] 
         * return a constant reference, and have the mutator version return the simple reference.
         *
         */
        const vector<Object> & operator[](int row) const 
        {
            return array[row];
        }

        vector<Object> & operator[](int row)
        {
            return array[row];
        }

        int row() const 
        {
            return array.size();
        }

        int column() const
        {
            return row()? array[0].size() : 0;
        }

    private:
        vector<vector<Object>> array;
};

#endif
