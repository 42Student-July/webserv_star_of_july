#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

namespace utility {
std::string trimCopyIf(const std::string& str, const std::string& set);
std::string toLower(const std::string& str);

template <typename T>
std::string toString(T& val);

template< class InputIt, class T >
InputIt myFind(InputIt first, InputIt last, const T& value);

}  // namespace utility

template <typename T>
std::string utility::toString(T& val) {
  std::stringstream ss;

  ss << val;
  return ss.str();
}

template< class InputIt, class T >
InputIt utility::myFind(InputIt first, InputIt last, const T& value)
{
	for (; first != last; ++first) {
		if (*first == value) {
			return first;
		}
	}
	return last;
}

#endif /* UTILITY_HPP */
