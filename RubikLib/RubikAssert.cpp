#include "RubikAssert.h"

#include <iostream>

void AsertFunc(const std::string & reason, const std::string & f, unsigned int l)
{
    //std::cout << "Assert at " << f << ":" << l << " ; " << reason << std::endl;; 
    throw std::logic_error(reason);
}

