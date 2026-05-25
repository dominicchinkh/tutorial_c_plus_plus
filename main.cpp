#include <iostream>

#include "src/book/data-structures-and-algorithm-analysis/chapter4/binary-search-tree.h"

using namespace std;

int main(int argc, char *argv[]) 
{
    BinarySearchTree<int> a;
    
    std::cout << "Binary search tree is " << (a.empty()? "empty" : "not empty") << std::endl;

    a.insert(4);
    a.insert(2);
    a.insert(6);
    a.insert(7);
    a.insert(1);

    std::cout << "Binary search tree is " << (a.empty()? "empty" : "not empty") << std::endl;

    a.printTree(); // expect 4, 2, 1, 6, 7

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
    a.printTree(); // expect 4, 2, 1, 7

    BinarySearchTree<int> b{a};
    b.printTree(); // expect 4, 2, 1, 7

    a.makeEmpty();
    std::cout << "Binary search tree is " << (a.empty()? "empty" : "not empty") << std::endl;

    return 0;
}
