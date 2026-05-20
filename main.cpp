#include <iostream>
#include <vector>

#include "src/Concept/Chronium/Logging.h"
// #include "src/DataStructuresAndAlgorithmAnalysis/chapter1/IntCell.h"
// #include "src/DataStructuresAndAlgorithmAnalysis/chapter1/Recursion.h"
// #include "src/DataStructuresAndAlgorithmAnalysis/chapter1/ClassTemplate.h"
// #include "src/DataStructuresAndAlgorithmAnalysis/chapter1/FunctionObject.h"
// #include "src/DataStructuresAndAlgorithmAnalysis/chapter1/FunctionTemplate.h"
// #include "src/DataStructuresAndAlgorithmAnalysis/chapter1/Matrix.h"
// #include "src/DataStructuresAndAlgorithmAnalysis/chapter1/Square.h"
// #include "src/DataStructuresAndAlgorithmAnalysis/chapter2/BinarySearch.h"
// #include "src/DataStructuresAndAlgorithmAnalysis/chapter2/Exponentiation.h"
// #include "src/DataStructuresAndAlgorithmAnalysis/chapter2/GreatestCommonDivisor.h"
// #include "src/DataStructuresAndAlgorithmAnalysis/chapter2/MaximumSubsequence.h"
// #include "src/EffectiveModernCPlusPlus/chapter1/TemplateTypeDeduction.h"
// #include "src/Experiment/Switch0.h"
// #include "src/Experiment/TokenPastingOperator.h"

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
