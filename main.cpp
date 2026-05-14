#include <iostream>
#include <vector>
// #include "src/chapter1/IntCell.h"
// #include "src/chapter1/Recursion.h"
// #include "src/chapter1/ClassTemplate.h"
#include "src/chapter1/FunctionObject.h"
// #include "src/chapter1/FunctionTemplate.h"
// #include "src/chapter1/Square.h"

using namespace std;

int main(int argc, char *argv[]) 
{
    vector<string> arr = {"Zebra", "alligator", "crocodile"};

    cout << findMax(arr, CaseInsensitiveCompare{}) << endl;
    cout << findMax(arr) << endl;

    return 0;
}
