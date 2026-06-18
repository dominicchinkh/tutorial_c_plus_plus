#pragma once

using namespace std;

/*
 * A class for simulating an integer memory cell
 *
 *--------------------------------------
 * Object Declaration and Initialization
 * 
 * This section demonstrates the standard ways to declare objects on the stack using both 
 * traditional functional syntax and the modern uniform initialization (brace initialization) 
 * introduced in C++11.
 * 
 *     IntCell obj1;        // Calls the default constructor
 *     IntCell obj2( 12 );  // Traditional functional-style initialization
 *     IntCell obj2{ 12 };  // Uniform (brace) initialization; preferred in modern C++ to 
 *                          // prevent narrowing conversions
 *     IntCell obj4{ };     // Modern syntax for calling the zero-parameter constructor
 * 
 *--------------------------------- 
 * Dynamic Memory Allocation (Heap)
 * 
 * These lines illustrate the use of the new keyword (implied by the pointer type) or the creation
 * of temporary objects that would typically be assigned to a pointer to be stored on the heap.
 * 
 *     IntCell* m = IntCell();  // Allocates an object on the heap using the default constructor
 *     IntCell* m = IntCell{};  // Allocates on the heap using uniform initialization
 *     IntCell* m = IntCell     
 * 
 *---------------------------------------
 * Illegal Declarations and Common Errors
 * 
 * This section highlights syntax that will either fail to compile or result in unintended behavior 
 * (like a function declaration instead of an object)
 * 
 *     IntCell obj3 = 37;   // This fails if the constructor is marked explicit, as it prevents 
 *                          // implicit conversion from int to IntCell
 *     IntCell obj4( );     // Known as the "Most Vexing Parse." The compiler interprets this as a 
 *                          // declaration for a function named obj4 that returns an IntCell, rather 
 *                          // than an object instantiation.
 * 
 *--------------------------
 * Stack vs. Heap Allocation
 * 
 * The two main functions demonstrate the fundamental difference between automatic and dynamic memory 
 * allocation in C++
 * 
 * Stack Allocation (Automatic Storage):
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
 * Heap Allocation (Dynamic Storage):
 * 
 *   int main()
 *   {
 *       IntCell* m = new IntCell{ 0 };
 * 
 *       m->write(5);
 *       cout << m->read() << endl;
 * 
 *       delete m;
 * 
 *       return 0;
 *   }
 * 
 *---------------------------------------
 * Copy Constructor and Move Constructor
 * 
 * These are two special constructors that are required to construct a new object, initialized
 * to the same state as another object of the same type.
 * 
 * A copy constructor or move constructor is called in the following instances
 * 
 * 1. A declaration with initialization
 * 
 *   IntCell B = C;   // Copy construct if C is lvalue; Move construct if C is rvalue
 *   IntCell B { C }; // Copy construct if C is lvalue; Move construct if C is rvalue
 * 
 * 2. An object passed using call-by-value
 * 3. An object returned by value
 * 
 *-------------------------------------
 * Copy Assignment and Move Assignment
 * 
 * The assignment operator is called when = is applied to two objects that have both been
 * previously constructed
 * 
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

        ~IntCell();
        IntCell(const IntCell & rhs);
        IntCell(IntCell && rhs);
        IntCell & operator=(const IntCell & rhs);
        IntCell & operator=(IntCell && rhs);

        // Make the function an accessor. The `const` after a closing parenthesis signifies an accessor (function 
        // that does not change the state of its object).
        int read() const;

        void write(int x);

        /*------------------
         * Parameter Passing
         * 
         * 1. If the formal parameter should be able to change the value of the artual argument, then you must use call
         *    by reference
         * 
         *    void swap(double& x, double& y);
         * 
         * 2. Otherwise, the value of the actual argument cannot be changed by the formal parameter.
         *    a. If the type is a primitive type, use call-by-value
         * 
         *       double average(double a, double b);
         * 
         *    b. Otherwise, the type is a class type and is generally passed using call-by-constant-reference, unless it
         *       is an unusually small and easily copyable type
         * 
         *        string randomItem(const vector<string>& arr);
         * 
         * 3. If the argument is an rvalue (a temporary about to be destroyed), use call-by-rvalue-reference.
         * 
         *    string randomItem(vector<string>&& arr);
         *    cout << randomItem( { "hello", "world" } ) << endl; // The caller must also use a constant reference to access 
         *                                                        // the return value, as shown at line 15; otherwise, there will
         *                                                        // still be a copy.
         */ 

        /*---------------
         * Return Passing
         * 
         * return-by-value
         *   1. Best for: Small primitive types (like int or double) or when returning a local object that will be destroyed when the 
         *      function exits.
         *   2. In modern C++ (C++11 and later), Move Semantics and Return Value Optimization (RVO) often eliminate the cost of the copy, 
         *      making this efficient even for large objects like std::vector or std::string.
         * 
         * return-by-constant-reference
         *   This returns a reference to an existing object without creating a copy, while also preventing the caller from modifying that object.
         * 
         *   1. Best for: Accessing large objects stored within a collection or class (like a getter method) to avoid the overhead of copying
         *   2. You must ensure the object being referenced outlives the reference itself. Never return a reference to a local variable
         * 
         *   const LargeType & randomItem2( const vector<LargeType> & arr );
         *   const LargeType & item3 = randomItem2( vec );
         * 
         * return-by-reference
         *   Similar to return-by-constant-reference, but it allows the caller to modify the original object.
         * 
         *   1. Best for: Overloading operators (like operator[] in a Vector class) where you want to allow syntax like `myVector[0] = newValue`;
         * 
         *   LargeType & getElement( vector<LargeType> & arr, int index ) {
         *       return arr[index];
         *   }
         */
    
    private:
        int* value;
};
