#include <iostream>
#include <vector>

#include "src/concept/chronium/logging.h"

using namespace std;

bool MyCustomLogCallback(int severity, const char* file, int line, 
                         size_t message_start, const std::string& str) 
{
    std::cout << "[CUSTOM HOOK] Caught a severity " << severity << " log: " << str;
    
    return true; // Return true to consume it (prevents it from going to standard destinations)
}

int main(int argc, char *argv[]) 
{
    g_min_log_level = LOGGING_WARNING;
    g_log_message_handler = MyCustomLogCallback;

    LOG(ERROR) << "Processing request for user: " << endl;
    return 0;
}
