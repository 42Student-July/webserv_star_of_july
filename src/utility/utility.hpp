#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <iostream>
#include <sstream>
#include <string>

namespace utility {
std::string trimCopyIf(const std::string& str, const std::string& set);
std::string toString(size_t val);
}  // namespace utility

#endif /* UTILITY_HPP */
