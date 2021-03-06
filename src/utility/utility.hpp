#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <algorithm>
#include <cerrno>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <string.h>

namespace utility {
std::string trimCopyIf(const std::string& str, const std::string& set);
std::string toLower(const std::string& str);
int stoi(const std::string& str, size_t* endpos = NULL, long base = 10);
int hexStringToInt(const std::string& hex_str);
void freeArrays(char** arrays);
char **map2Array(std::map<std::string, std::string> map_env,
                 std::string connector);

template <typename T>
std::string toString(T& val);

template <typename T>
int toInt(T& val);

template <typename T>
void print_vector(std::vector<T> vec);

//  <algorithm>のstd::all_of(c++11)を再実装
template <class InputIt, class UnaryPredicate>
bool all_of(InputIt first, InputIt last, UnaryPredicate p);
}  // namespace utility

template <typename T>
std::string utility::toString(T& val) {
  std::stringstream ss;

  ss << val;
  return ss.str();
}

template <typename T>
int utility::toInt(T& val) {
  std::stringstream ss;
  int num;

  ss << val;
  ss >> num;
  return num;
}

// https://github.com/llvm-mirror/libcxx/blob/a12cb9d211019d99b5875b6d8034617cbc24c2cc/include/algorithm#l838
template <class InputIt, class UnaryPredicate>
bool utility::all_of(InputIt first, InputIt last, UnaryPredicate p) {
  for (; first != last; ++first) {
    if (!p(*first)) {
      return false;
    }
  }
  return true;
}

template <typename T>
void utility::print_vector(std::vector<T> vec) {
  typename std::vector<T>::iterator it = vec.begin();
  int i = 0;
  for (; it < vec.end(); it++) {
    i++;
    std::cout << i << "] " << *it << std::endl;
  }
}

#endif /* UTILITY_HPP */
