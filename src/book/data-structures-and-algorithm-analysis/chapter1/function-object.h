#pragma once

#include <functional>
#include <string>
#include <vector>
#include "string.h"

using namespace std;

/*
 *  A function object `CaseInsensitiveCompare` and how it's used
 *  
 *--------------------------------------
 * Object Declaration and Initialization
 * 
 *   vector<string> arr = {"Zebra", "alligator", "crocodile"};
 *   cout << findMax(arr, CaseInsensitiveCompare{}) << endl;
 *   cout << findMax(arr) << endl;
 * 
 */

// Generic `findMax`, with a function object
template <typename Object, typename Comparator>
const Object & findMax(const vector<Object> & arr, Comparator isLessThan)
{
    int maxIndex = 0;

    for(int i = 0; i < arr.size(); i++) {
        if( isLessThan(arr[maxIndex], arr[i]) ) {
            maxIndex = i;
        }
    }

    return arr[maxIndex];
}

// Generic `findMax`, using default ordering
template <typename Object>
const Object & findMax(const vector<Object> & arr)
{
    return findMax(arr, less<Object>{});
}

class CaseInsensitiveCompare
{
    public:
        bool operator()(const string &lhs, const string &rhs) {
            return strcasecmp(lhs.c_str(), rhs.c_str()) < 0;
        }
};
