#pragma once

#include <vector>

using namespace std;

/*
 *  A function for finding maximum value in a vector
 *  
 *--------------------------------------
 * Object Declaration and Initialization
 * 
 *    vector<int> n = {3, 2, 4, 6, 9};
 *    cout << findMax<int>(n) << endl;
 * 
 */

template <typename Comparable>
const Comparable & findMax(const vector<Comparable> & a)
{
    int maxIndex = 0;

    for(int i = 1; i < a.size(); i++) {
        if (a[i] > a[maxIndex]) {
            maxIndex = i;
        }
    }

    return a[maxIndex];
}
