#ifndef INTCELL_H
#define INTCELL

/*
 * A class for simulating an integer memory cell
 *
 * Declaration:
 *   Legal:
 *     IntCell obj1;        // Zero parameter constructor
 *     IntCell obj2( 12 );  // One parameter constructor
 *     IntCell obj2{ 12 };  // One parameter constructor
 *     IntCell obj4{ };     // Zero parameter constructor
 * 
 *   Illegal:
 *     IntCell obj3 = 37;   // Constructor is explicit
 *     IntCell obj4( );     // Function declaration
 * 
 *   int main() 
 *   {
 *       IntCell m;
 *   
 *       m.write(5);
 *       cout << m.read() << endl;
 *   
 *       return 0;
 *   }
 * 
 *   int main()
 *   {
 *       IntCell* m = new IntCell{ 0 };
 * 
 *       m->write(5);
 *       cout << m->read() << endl;
 * 
 *       return 0;
 *   }
 */
class IntCell
{
    public:
        // 1. Make one-parameter constructor explicit to avoid behind-the-scenes type conversions. 
        // 2. Use default parameter.
        // 3. Use initializatoin list to initialize the data member.
        //    + Save time in the case where the data memebers are class types that have complex initialization.
        //    + If the data member is `const`
        //    + If the data member is itself a class type that does not have a zero-parameter constructor

        // Take note the use of braces (uniform syntaz for initialization) instead of parentheses in  
        // initialization list

        explicit IntCell(int initialValue = 0);

        // Make the function an accessor. The `const` after a closing parenthesis signifies an accessor (function 
        // that does not change the state of its object).
        int read() const;

        void write(int x);
    
    private:
        int value;
};

#endif