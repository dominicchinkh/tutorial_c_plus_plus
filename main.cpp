#include <iostream>

#include "src/book/data-structures-and-algorithm-analysis/chapter3/array-queue.h"

using namespace std;

int main(int argc, char *argv[]) 
{
    ArrayQueue<int> a;

    a.enqueue(34);
    a.enqueue(65);
    a.enqueue(98);

    // a.dequeue();
    // a.dequeue();
    // a.dequeue();

    // std::cout << a.dequeue() << std::endl;
    // std::cout << a.dequeue() << std::endl;
    // std::cout << a.dequeue() << std::endl;

    // ArrayQueue copy
    ArrayQueue<int> b(a);

    std::cout << b.dequeue() << std::endl;
    std::cout << b.dequeue() << std::endl;
    std::cout << b.dequeue() << std::endl;

    return 0;
}
