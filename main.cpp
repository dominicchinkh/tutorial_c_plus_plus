#include <iostream>

#include "src/book/data-structures-and-algorithm-analysis/chapter5/cuckoo-hash-table.h"
#include "src/book/data-structures-and-algorithm-analysis/chapter5/hash-family.h"

using namespace std;

int main(int argc, char *argv[]) 
{
    CuckooHashTable<string, StringHashFamily<7>> t;

    t.insert("window");
    t.insert("linux");
    t.insert("macos");

    std::cout << "Contain 'window' (before remove)? " << (t.contains("window")? "YES" : "NO") << std::endl;
    t.remove("window");
    std::cout << "Contain 'window' (after remove)? " << (t.contains("window")? "YES" : "NO") << std::endl;

    return 0;
}
