#include <iostream>

#include "utility.hpp"

int main() {
  std::string str("42toKyoDayO!!!af");
  std::string lower_str = utility::toLower(str);
  std::cout << lower_str << std::endl;
}
