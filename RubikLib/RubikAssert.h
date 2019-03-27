#pragma once

#include <string>

void AsertFunc(const std::string & reason, const std::string & f, unsigned int l);

#ifdef NDEBUG
#define RASSERT(pred, str) nullptr
#else // NDEBUG
#define RASSERT(pred, str) if (!(pred)) {AsertFunc(str, __FILE__, __LINE__);}
#endif // NDEBUG

#define Throw(str)     throw std::logic_error(str)
