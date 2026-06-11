#include <iostream>

#include "src/book/data-structures-and-algorithm-analysis/chapter5/probing-hash-table.h"

using namespace std;

int main(int argc, char *argv[]) 
{
    ProbingHashTable<int> t;

    t.insert(47);
    t.insert(13);
    t.insert(81);
    t.insert(95);
    t.insert(42);

    std::cout << "Contain 81? " << (t.contains(81)? "YES": "NO") << std::endl;
    std::cout << "Current size: " << t.size() << std::endl;

    t.remove(81);

    std::cout << "Contain 81? " << (t.contains(81)? "YES": "NO") << std::endl;
    std::cout << "Current size: " << t.size() << std::endl;
}
