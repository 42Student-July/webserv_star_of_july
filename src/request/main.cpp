
#include <fstream>

#include "HttpRequest.hpp"
#include "HttpRequestParser.hpp"

std::string readFile(const char *filepath) {
  std::ifstream ifs(filepath);

  if (ifs.fail()) {
    std::cerr << "ifstream:open failed()";
    exit(EXIT_FAILURE);
  }
  std::string file_content((std::istreambuf_iterator<char>(ifs)),
                           std::istreambuf_iterator<char>());
  return file_content;
}

int main() {
  HttpRequestParser parser;
  std::string file_content = readFile("tests/gtest/request/simple_get.crlf");
  HttpRequest *request = parser.parse(file_content.c_str());

  std::cout << *request;
}
