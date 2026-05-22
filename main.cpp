#include <iostream>
#include <vector>

#include "src/book/data-structures-and-algorithm-analysis/chapter3/linked-list-stack.h"

using namespace std;

int main(int argc, char *argv[]) 
{
    LinkedListStack<int> a;

    cout << "Current size: " << a.size() << endl;
    cout << "Is empty? " << (a.empty() ? 'Y':'N') << endl;
   
    a.push(47);
    a.push(19);
    a.push(73);

    cout << "`a` new size: " << a.size() << endl;
    cout << "Is empty? " << (a.empty() ? 'Y':'N') << endl;
    
    for (; !a.empty();) {
        cout << "`a` element: " << a.pop() << endl;
    }

    return 0;
}
