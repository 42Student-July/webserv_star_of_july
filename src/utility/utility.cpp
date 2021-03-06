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

int utility::stoi(const std::string& str, size_t* endpos, long base) {
  const char* p = str.c_str();
  char* end;
  errno = 0;
  long num = std::strtol(p, &end, base);
  if (p == end) {
    throw std::invalid_argument("stoi");
  }
  if (errno == ERANGE || num < std::numeric_limits<int>::min() ||
      num > std::numeric_limits<int>::max()) {
    throw std::out_of_range("stoi");
  }
  if (endpos != NULL) {
    *endpos = static_cast<std::size_t>(end - p);
  }
  return num;
}

static bool consistsOfHexadecimal(const std::string& str) {
  return utility::all_of(str.begin(), str.end(), isxdigit);
}

// 16進数の文字列をintに変換する。
// 0123456789abcdefABCDEF以外の文字を許容しない
int utility::hexStringToInt(const std::string& hex_str) {
  if (!consistsOfHexadecimal(hex_str)) {
    throw std::invalid_argument("hexStringToInt");
  }
  return utility::stoi(hex_str, NULL, 16);
}

void utility::freeArrays(char** arrays) {
  size_t idx = 0;

  if (!arrays) return;
  while (arrays[idx]) {
    free(arrays[idx]);
    idx += 1;
  }
  free(arrays);
}

char** utility::map2Array(std::map<std::string, std::string> map_env,
                          std::string connector) {
  char** array_env = NULL;
  array_env = new char*[map_env.size() + 1];
  size_t i = 0;
  std::map<std::string, std::string>::const_iterator it = map_env.begin();
  for (; it != map_env.end(); it++, i++) {
    std::string env_str = it->first + connector + it->second;
    array_env[i] = strdup(env_str.c_str());
  }
  array_env[i] = NULL;
  return array_env;
}
