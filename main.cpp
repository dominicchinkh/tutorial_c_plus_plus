#include <iostream>

// #include "src/book/data-structures-and-algorithm-analysis/chapter3/vector.h"
#include "src/book/data-structures-and-algorithm-analysis/chapter4/set.h"

using namespace std;

int main(int argc, char *argv[]) 
{
    Set<int> a(3);
    a[0] = 34;
    a[1] = 56;
    a[2] = 98;

    Set<int> b(a);
    Set<int> c = a;

    cout << "b: " << b[0] << ' ' << b[1] << ' ' << b[2] << endl;
    cout << "c: " << c[0] << ' ' << c[1] << ' ' << c[2] << endl;

    cout << "Current size: " << a.size() << endl;
    cout << "Current capacity: " << a.capacity() << endl;

    a.insert(47);
    cout << "New element at index " << (a.size() - 1) << ": "  << a[a.size() - 1] << endl;

    a.print();

    a.erase(56);
    cout << "a new size after erase: " << a.size() << endl;

    a.print();

    a.reserve(20);

    cout << "New size after reserve: " << a.size() << endl;
    cout << "New capacity after reserve: " << a.capacity() << endl;

    Set<int>::iterator itr = a.begin();

    cout << "a elements: " << endl;
    for (Set<int>::iterator itr = a.begin(); itr != a.end(); ++itr) {
        cout << *(itr) << endl;
    }

    a.print();
    
    return 0;
}
