#ifndef SWITCH_0_H
#define SWITCH_0_H

#include <iostream>
#include <string>

using namespace std;

/*
 *  https://github.com/dominicchinkh/tutorial_c_plus_plus/wiki/C---Switch(0)-Scoping-Trick
 *
 *  int x = 5, y = 8;
 *  SMART_CHECK_EQ(x, y);
 */

// A mock class to represent Chronium's LogMessage
struct MockLogMessage {
    string message;
    void Panic() { 
        cout << "CRASH: " << message << endl; 
        exit(1);
    }
};

#define SMART_CHECK_EQ(val1, val2)                                  \
    switch(0)                                                       \
        case(0):                                                    \
        default:                                                    \
            if (val1 == val2)                                       \
                ;                                                   \
            else                                                    \
                for (MockLogMessage msg{"Values are not equal!"};;) \
                    for (msg.Panic();;)

// Note: The double 'for' loop at the end is a way to execute msg.Panic() 
// and immediately halt/exit the macro execution block.

#endif