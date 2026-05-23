#include <iostream>

#include "src/book/data-structures-and-algorithm-analysis/chapter3/postfix.h"

using namespace std;

int main(int argc, char *argv[]) 
{
    Postfix pf;
    
    // Single digit Numbers
    std::cout << "'5 3 +' (8): "  << (pf.parseString("5 3 +") == 8? 'Y' : 'N') << std::endl;
    std::cout << "'5 3 -' (2): "  << (pf.parseString("5 3 -") == 2? 'Y' : 'N') << std::endl;
    std::cout << "'5 3 *' (15): " << (pf.parseString("5 3 *") == 15? 'Y' : 'N') << std::endl;

    // Multiple digit Numbers
    std::cout << "'25 13 +' (38): "  << (pf.parseString("25 13 +") == 38? 'Y' : 'N') << std::endl;
    std::cout << "'25 13 -' (12): "  << (pf.parseString("25 13 -") == 12? 'Y' : 'N') << std::endl;
    std::cout << "'25 13 *' (325): " << (pf.parseString("25 13 *") == 325? 'Y' : 'N') << std::endl;

    // Edge case 1: the input string ends with a number
    std::cout << "'52': "  << (pf.parseString("52") == 52? 'Y' : 'N') << std::endl;

    // Edge case 2: there is no space between a number and a valid operator
    std::cout << "'5 3+' (8): "  << (pf.parseString("5 3+") == 8? 'Y' : 'N') << std::endl;

    // Edge case 3: the stack has more than one element at the end of the process
    try {
        pf.parseString("34 89");
    }
    catch(const std::overflow_error & e) {
        std::cout << "Overflow error thrown for '34 89'" << std::endl;
    }

    // Edge case 4: the user types an invalid character like `?`
    try {
        pf.parseString("34 ?");
    }
    catch(const std::invalid_argument & e) {
        std::cout << "Invalid argument error thrown for '34 ?'" << std::endl;
    }

    // Edge case 5: the stack has less than two elements before popping for an operator
    try {
        pf.parseString("5 +");
    }
    catch (const std::underflow_error & e) {
        std::cout << "Underflow error thrown for '5 +'" << std::endl;
    }

    // Edge case 6: the string is empty or contains only spaces
    try {
        pf.parseString(" ");
    }
    catch (const std::underflow_error & e) {
        std::cout << "Underflow error thrown for ' '" << std::endl;
    }

    return 0;
}
