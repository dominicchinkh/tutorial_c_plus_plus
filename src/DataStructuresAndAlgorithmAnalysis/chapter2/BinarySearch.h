/*
 * Given an integer X and integers A0, A1, . . . , AN−1, which are presorted and already in
 * memory, find i such that Ai = X, or return i = −1 if X is not in the input.
 *
 */

// vector<int> a = {-5, -4, -2, 3, 11, 13};
// cout << binarySearch(a, -5) << endl;
// cout << binarySearch(a, -4) << endl;
// cout << binarySearch(a, -2) << endl;
// cout << binarySearch(a,  3) << endl;
// cout << binarySearch(a, 11) << endl;
// cout << binarySearch(a, 13) << endl;

#include <vector>

using namespace std;

#define NOT_FOUND -1

template <typename Comparable>
int binarySearch(const vector<Comparable> & a, const Comparable & x)
{
    int low = 0, high = a.size() - 1;

    while (low <= high) {

        int mid = (low + high) / 2;

        if (a[mid] < x) {
            low = mid + 1;
        }
        else if (a[mid] > x) {
            high = mid - 1;
        }
        else {
            return mid;
        }
    }

    return NOT_FOUND;
}
