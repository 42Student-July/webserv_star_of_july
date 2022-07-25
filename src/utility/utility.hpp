#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <algorithm>
#include <cerrno>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

namespace utility {
std::string trimCopyIf(const std::string& str, const std::string& set);
std::string toLower(const std::string& str);
int stoi(const std::string& str, size_t* endpos = NULL, long base = 10);
bool isHexDigitString(const std::string& str);
int hexStringToInt(const std::string& hex_str);

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
