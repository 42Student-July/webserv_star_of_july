#include "utility.hpp"

std::string utility::trimCopyIf(const std::string& str,
                                const std::string& set) {
  if (str.empty() || set.empty()) {
    return str;
  }

  std::string::size_type begin = str.find_first_not_of(set);
  std::string::size_type end = str.find_last_not_of(set);
  size_t len = end - begin + 1;

  if (begin == std::string::npos) {
    return "";
  }
  return str.substr(begin, len);
}

std::string utility::toLower(const std::string& str) {
  std::string lower_str;

  lower_str.resize(str.size());
  std::transform(str.begin(), str.end(), lower_str.begin(), tolower);
  return lower_str;
}
