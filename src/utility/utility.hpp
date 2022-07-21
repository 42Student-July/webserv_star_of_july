#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <iostream>
#include <sstream>
#include <string>

namespace utility {
std::string trimCopyIf(const std::string& str, const std::string& set);
template <typename T>
std::string toString(T& val);
}  // namespace utility

template <typename T>
std::string utility::toString(T& val) {
  std::stringstream ss;

  ss << val;
  return ss.str();
}
#endif /* UTILITY_HPP */
