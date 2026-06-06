#include <iostream>

// #include "src/book/data-structures-and-algorithm-analysis/chapter3/vector.h"
#include "src/book/data-structures-and-algorithm-analysis/chapter4/binary-search-tree-set.h"

using namespace std;

int main(int argc, char *argv[]) 
{
    BinarySearchTreeSet<int> a;

    std::cout << "Binary search tree is " << (a.empty()? "empty" : "not empty") << std::endl;

    a.insert(4);
    a.insert(2);
    a.insert(6);
    a.insert(7);
    a.insert(1);

    std::cout << "After insert, binary search tree is " << (a.empty()? "empty" : "not empty") << std::endl;

    a.printTree(); // expect 1, 2, 4, 6, 7

    if (auto minVal = a.findMin()) {
        std::cout << "The minimum value is: " << *minVal << std::endl;
    } else {
        std::cout << "The structure is empty, no minimum found." << std::endl;
    }

    if (auto maxVal = a.findMax()) {
        std::cout << "The maximum value is: " << *maxVal << std::endl;
    } else {
        std::cout << "The structure is empty, no maximum found." << std::endl;
    }

    a.remove(6);

    a.printTree(); // expect 1, 2, 4, 7

    auto itr = a.begin();

    int i = 0;

    cout << "a elements: " << endl;
    for (auto itr = a.begin(); itr != a.end(); ++itr) {
        cout << *(itr) << endl;

        if(++i > 5) {
            break;
        }
    }

    BinarySearchTreeSet<int> b{a};

    cout << "The value of `b`: ";
    b.printTree(); // expect 1, 2, 4, 7
    
    a.makeEmpty();
    std::cout << "After `makeEmpty`, Binary search tree is " << (a.empty()? "empty" : "not empty") << std::endl;

    return 0;
}
