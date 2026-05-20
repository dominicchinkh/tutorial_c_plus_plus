#ifndef CLASS_TEMPLATE_H
#define CLASS_TEMPLATE_H

/*
 *  A class for finding storing a value
 *  
 *  Note: Object must have a zero-parameter constructor, a copy constructor, and a copy 
 *        assignment operator
 *
 *--------------------------------------
 * Object Declaration and Initialization
 * 
 *    MemoryCell<int> m1;
 *    m1.write(48);
 *    cout << m1.read() << endl;  
 * 
 *    MemoryCell<string> m1;
 *    m1.write("dominic");
 *    cout << m1.read() << endl;  
 * 
 */

template <typename Object>
class MemoryCell
{
    public:
        explicit MemoryCell(const Object & initialValue = Object{}): value {initialValue} {}
        const Object & read() const {
            return value;
        }
        void write(const Object & x) {
            value = x;
        }
    private:
        Object value;
};

#endif
