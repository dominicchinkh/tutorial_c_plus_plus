#include <iostream>
#include "src/chapter1/IntCell.h"
// #include "src/chapter1/Recursion.h"

using namespace std;

int main(int argc, char *argv[]) 
{
    IntCell* m = new IntCell{ 0 };

    m->write(5);
    cout << m->read() << endl;
    
    delete m;
    
    return 0;
}
