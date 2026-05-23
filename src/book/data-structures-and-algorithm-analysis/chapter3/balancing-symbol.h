#ifndef BALANCING_SYMBOL_H
#define BALANCING_SYMBOL_H

/*
 * Compilers check your programs for syntax errors, but frequently a lack of one symbol (such 
 * as a missing brace or comment starter) can cause the compiler to spill out a hundred lines 
 * of diagnostics without identifying the real error. 
 * 
 * A useful tool in this situation is a program that checks whether everything is balanced.
 * Thus, every right brace, bracket, and parenthesis must correspond to its left counterpart.
 * 
 * The simple algorithm uses a stack and is as follows:
 * 
 *   Make an empty stack. Read characters until end of string. 
 *   
 *     + If the character is an opening symbol, push it onto the stack. 
 *
 *     x If it is a closing symbol and the stack is empty, report an error. 
 *     + Otherwise, pop the stack. 
 * 
 *       x If the symbol popped is not the corresponding opening symbol, then report an error.
 *       x At end of string, if the stack is not empty, report an error.
 * 
 */

/*
 *  int main(int argc, char *argv[]) 
 *  {
 *      BalancingSymbol bs;
 *       try {
 *          bs.parseString(" [ ( { } )]");
 *      }
 *      catch (const std::runtime_error & e) {
 *          std::cerr << "Caught exception: " << e.what() << std::endl;
 *      }
 *  
 *      std::cout << "Expect: exception: Stack is empty" << std::endl;
 *  
 *      try {
 *          bs.parseString(" )]");
 *      }
 *      catch (const std::runtime_error & e) {
 *          std::cerr << "  Caught exception: " << e.what() << std::endl;
 *      }
 *  
 *      std::cout << "Expect: exception: Not the corresponding opening symbol" << std::endl;
 *  
 *      try {
 *          bs.parseString(" [ ( { )]");
 *      }
 *      catch (const std::runtime_error & e) {
 *          std::cerr << "  Caught exception: " << e.what() << std::endl;
 *      }
 *  
 *      std::cout << "Expect: exception: The stack is not empty at end of string" << std::endl;
 *  
 *      try {
 *          bs.parseString("[ [ ( { } )]");
 *      }
 *      catch (const std::runtime_error & e) {
 *          std::cerr << "  Caught exception: " << e.what() << std::endl;
 *      }
 *  
 *      return 0;
 *  }
 */
#include <stdexcept>
#include <string>

#include "array-stack.h"

class BalancingSymbol
{
    public:
        void parseString(const std::string & text)
        {
            ArrayStack<char> theArray;
            
            for (const char & ch: text) {
                /*
                 * The static_cast<unsigned char>(ch) is required to prevent undefined behaviour (which can lead 
                 * to memory corruption or crashes) when passing a char to functions from the legacy C library, 
                 * such as std::isspace.
                 *
                 * In C++, standard char is often a signed 8-bit integer on most platforms. A signed char can hold
                 * values from -128 to 127. However, std::isspace expects an integer input that fits inside an 
                 * unsigned char (0 to 255), or the special constant EOF (-1).
                 *
                 * If your string contains an extended ASCII or UTF-8 character (like é, ß, or ÿ), its value will
                 * fall in the 128 to 255 range. Because char is signed, C++ interprets these characters as 
                 * negative numbers (e.g., -61).
                 *
                 * If you pass a negative number like -61 into that array without casting, the program attempts 
                 * to read a memory address before the start of the table (_IsSpaceTable[-61]). This results in 
                 * an out-of-bounds array access, causing unpredictable bugs or program crashes.
                 */

                if (std::isspace(static_cast<unsigned char>(ch))) {
                    continue;
                }

                // If the character is an opening symbol, push it onto the stack
                if (ch == '[' || ch == '(' || ch == '{') {
                    theArray.push(ch);
                }

                // If it is a closing symbol
                else if (ch == ']' || ch == ')' || ch == '}') {
                    
                    // If the stack is empty, report an error
                    if (theArray.empty()) {
                        throw std::runtime_error("Stack is empty");
                    }
                    else {
                        // Otherwise, pop the stack
                        char c = theArray.pop();

                        // If the symbol popped is not the corresponding opening symbol, then report an error
                        if ((ch == ']' && c != '[') || (ch == ')' && c != '(') || (ch == '}' && c != '{')) {

                            // I cannot use the + operator if the very first two items are a string literal. This is 
                            // because a string literal is treated as a const char* pointer, and C++ does not know how 
                            // to add a character value to a pointer in this context. 

                            // So I cast the opening string literal to a std::string explicitly.
                            
                            throw std::runtime_error(
                                std::string("Not the corresponding opening symbol: ") + c + ' ' + ch);
                        }
                    }
                }
            }

            // At end of string, if the stack is not empty, report an error.
            if (!theArray.empty()) {
                throw std::runtime_error("The stack is not empty at end of string");
            }
        }
};

#endif
