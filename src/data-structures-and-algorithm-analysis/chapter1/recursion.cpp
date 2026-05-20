#include <iostream>
#include "recursion.h"

using namespace std;

/*
 * 1. Base cases: You must always have some base cases, which can be solved without recursion.
 *
 * 2. Making progress: For the case that are to be solved recursively, the recursive call must 
 *    always be to a case that makes progress toward a base case.
 * 
 * 3. Design rulee: Assume that all recursive calls work.
 * 
 * 4. Compound interest rule: Never duplicate work by solving the same instance of a problem in
 *    separate recursive calls.
 * 
 */
int f(int x)
{
    if (x == 0) {
        return 0;
    }
    else {
        return 2 * f(x - 1) + x * x;
    }
}

void printDigit(int n) {
    // Take a single digit number and output it
    cout << n << endl;
}

void printOut(int n) {
    if (n >= 10) {
        printOut(n / 10);
    }
    
    // printDigit(n % 10);
    printDigit(n - n / 10 * 10);
}
