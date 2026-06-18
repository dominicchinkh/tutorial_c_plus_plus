#pragma once

/*
 * Postfix notation, or Reverse Polish Notation (RPN), is a mathematical method where operators 
 * follow their operands, eliminating the need for parentheses and operator precedence rules. For 
 * example 5 + 3, becomes 5 3 +. It is highly efficient for stack-based computer evaluation.
 * 
 */
#include <iostream>
#include <stdexcept>

#include "array-stack.h"

/*
 *  int main(int argc, char *argv[]) 
 *  {
 *      Postfix pf;
 *      
 *      // Single digit Numbers
 *      std::cout << "'5 3 +' (8): "  << (pf.parseString("5 3 +") == 8? 'Y' : 'N') << std::endl;
 *      std::cout << "'5 3 -' (2): "  << (pf.parseString("5 3 -") == 2? 'Y' : 'N') << std::endl;
 *      std::cout << "'5 3 *' (15): " << (pf.parseString("5 3 *") == 15? 'Y' : 'N') << std::endl;
 *  
 *      // Multiple digit Numbers
 *      std::cout << "'25 13 +' (38): "  << (pf.parseString("25 13 +") == 38? 'Y' : 'N') << std::endl;
 *      std::cout << "'25 13 -' (12): "  << (pf.parseString("25 13 -") == 12? 'Y' : 'N') << std::endl;
 *      std::cout << "'25 13 *' (325): " << (pf.parseString("25 13 *") == 325? 'Y' : 'N') << std::endl;
 *  
 *      // Edge case 1: the input string ends with a number
 *      std::cout << "'52': "  << (pf.parseString("52") == 52? 'Y' : 'N') << std::endl;
 *  
 *      // Edge case 2: there is no space between a number and a valid operator
 *      std::cout << "'5 3+' (8): "  << (pf.parseString("5 3+") == 8? 'Y' : 'N') << std::endl;
 *  
 *      // Edge case 3: the stack has more than one element at the end of the process
 *      try {
 *          pf.parseString("34 89");
 *      }
 *      catch(const std::overflow_error & e) {
 *          std::cout << "Overflow error thrown for '34 89'" << std::endl;
 *      }
 *  
 *      // Edge case 4: the user types an invalid character like ?
 *      try {
 *          pf.parseString("34 ?");
 *      }
 *      catch(const std::invalid_argument & e) {
 *          std::cout << "Invalid argument error thrown for '34 ?'" << std::endl;
 *      }
 *  
 *      // Edge case 5: the stack has less than two elements before popping for an operator
 *      try {
 *          pf.parseString("5 +");
 *      }
 *      catch (const std::underflow_error & e) {
 *          std::cout << "Underflow error thrown for '5 +'" << std::endl;
 *      }
 *  
 *      // Edge case 6: the string is empty or contains only spaces
 *      try {
 *          pf.parseString(" ");
 *      }
 *      catch (const std::underflow_error & e) {
 *          std::cout << "Underflow error thrown for ' '" << std::endl;
 *      }
 *  
 *      return 0;
 *  }
 */
class Postfix
{
    public:
        int parseString(const std::string & text)
        {
            ArrayStack<int> theArray;

            bool isNumber = false;
            int number = 0;

            for(const char & ch: text) {

                if (std::isspace(static_cast<unsigned char>(ch))) {

                    if (isNumber) {
                        theArray.push(number);
                        isNumber = false;
                        number = 0;
                    }
                    
                    continue;
                }

                if (std::isdigit(static_cast<unsigned char>(ch))) {

                    if (isNumber) {
                        number *= 10;
                    }

                    number += ch - '0';
                    isNumber = true;
                }
                else {
                    if (isNumber) {
                        theArray.push(number);
                        isNumber = false;
                        number = 0;
                    }

                    // Edge case: if a user types an invalid character like `?`
                    if (ch != '+' && ch != '*' && ch != '-') {
                        throw std::invalid_argument("Unknown or unsupported operator encountered.");
                    }

                    if (theArray.size() < 2) {
                        throw std::underflow_error(
                            "The stack has at least two elements before popping for an operator."
                        );
                    }

                    /*
                     * Stack Order: The top item on the stack is always the second number encountered in the 
                     * expression (the right operand). The item below it is the first number (the left operand).
                     * 
                     * Future Proofing: If you evaluate 5 3 -, the stack holds 5 (bottom) and 3 (top). To get 
                     * 5 - 3 = 2, you must explicitly assign right = 3 and left = 5.
                     */
                    int right = theArray.pop();
                    int left  = theArray.pop();
                    
                    switch(ch) {
                        case '+': theArray.push(left + right); break;
                        case '*': theArray.push(left * right); break;
                        case '-': theArray.push(left - right); break;

                        // Ignore division for now
                    }
                }
            }

            // Edge case: if the input string ends with a number (e.g., "5 3 + 10")
            if(isNumber) {
                theArray.push(number);
            }

            if (theArray.empty()) {
                throw std::underflow_error("There are no number on the stack");
            }

            if (theArray.size() > 1) {
                throw std::overflow_error("There are more than 1 number on the stack");
            }

            return theArray.pop();
        }
};
