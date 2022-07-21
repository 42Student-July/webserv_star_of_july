#include <iostream>

#include "utility.hpp"

int main() {
  std::string str("  42tokyo\t\t");
  std::cout << utility::trimCopyIf(str, " \t");
}
