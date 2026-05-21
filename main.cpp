#include <iostream>
#include <vector>

#include "src/book/data-structures-and-algorithm-analysis/chapter3/vector.h"

using namespace std;

int main(int argc, char *argv[]) 
{
    Vector<int> a(3);
    a[0] = 34;
    a[1] = 56;
    a[2] = 98;

    Vector<int> b(a);
    Vector<int> c = a;

    cout << "b: " << b[0] << ' ' << b[1] << ' ' << b[2] << endl;
    cout << "c: " << c[0] << ' ' << c[1] << ' ' << c[2] << endl;

    cout << "Current size: " << a.size() << endl;
    cout << "Current capacity: " << a.capacity() << endl;

    a.resize(6);
    a.reserve(20);

    cout << "New size: " << a.size() << endl;
    cout << "New capacity: " << a.capacity() << endl;
    
    a.push_back(47);
    cout << "New element at index " << (a.size() - 1) << ": "  << a[a.size() - 1] << endl;

    a.pop_back();
    cout << "a new size after pop_back: " << a.size() << endl;
    cout << "a last element after pop_back: " << a.back() << endl;

    Vector<int>::iterator itr = a.begin();

    cout << "a elements: " << endl;
    for (Vector<int>::iterator itr = a.begin(); itr != a.end(); ++itr) {
        cout << *(itr) << endl;
    }
    
    return 0;
}
