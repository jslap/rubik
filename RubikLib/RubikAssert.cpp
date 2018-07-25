#include "RubikAssert.h"

void AsertFunc(const std::string & reason, const std::string & f, unsigned int l)
{
    std::string assertStr = "Assert at " + f + ":" + std::to_string(l) + " ; " + reason; 
    throw std::logic_error(assertStr);
}

