#include <iostream>
#include <vector>

#include "src/book/data-structures-and-algorithm-analysis/chapter3/list.h"

using namespace std;

int main(int argc, char *argv[]) 
{
    List<int> a;

    cout << "Current size: " << a.size() << endl;
    cout << "Is empty? " << (a.empty() ? 'Y':'N') << endl;
   
    a.push_back(47);
    a.push_front(19);
    a.push_back(73);

    cout << "a new size: " << a.size() << endl;
    cout << "a first element: " << a.front() << endl;
    cout << "a last element: " << a.back() << endl;
    cout << "Is empty? " << (a.empty() ? 'Y':'N') << endl;

    a.pop_back();
    a.pop_front();

    cout << "a new size: " << a.size() << endl;
    cout << "a first element: " << a.front() << endl;
    cout << "a last element: " << a.back() << endl;
    cout << "Is empty? " << (a.empty() ? 'Y':'N') << endl;
    
    List<int>::iterator itr = a.begin();

    cout << "a elements: " << endl;
    for (auto itr = a.begin(); itr != a.end(); ++itr) {
        cout << *(itr) << endl;
    }

    return 0;
}
