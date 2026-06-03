#include <iostream>

#include "src/book/data-structures-and-algorithm-analysis/chapter4/avl-tree.h"

using namespace std;

int main(int argc, char *argv[]) 
{
    AvlTree<int> a;

    std::cout << "AVL tree is " << (a.empty()? "empty" : "not empty") << std::endl;

    a.insert(10);
    a.insert(11);
    a.insert(12);
    a.insert(13);
    a.insert(14);
    a.insert(15);
    a.insert(16);
    a.insert(8);
    a.insert(9);

    std::cout << "AVL tree is " << (a.empty()? "empty" : "not empty") << std::endl;

    a.printTree(); // expect 13, 11, 9, 8, 10, 12, 15, 14, 16

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

    a.remove(11);
    a.printTree(); // expect 13, 9, 8, 12, 10, 15, 14, 16

    AvlTree<int> b{a};
    b.printTree(); // expect 13, 9, 8, 12, 10, 15, 14, 16
    a.makeEmpty();
    std::cout << "AVL tree is " << (a.empty()? "empty" : "not empty") << std::endl;

    return 0;
}
