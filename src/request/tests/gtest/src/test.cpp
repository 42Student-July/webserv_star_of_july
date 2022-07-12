#include <gtest/gtest.h>

#include <fstream>

#include "HttpRequestDTO.hpp"
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

TEST(HttpParserTest, Simpletest) {
  HttpRequestParser parser;
  std::string file_content = readFile("gtest/request/simple_get.crlf");

  HttpRequestDTO *request = parser.parse(file_content.c_str());
  ASSERT_STREQ("GET", request->method.c_str());
  ASSERT_STREQ("/", request->uri.c_str());
  ASSERT_STREQ("HTTP/1.1", request->version.c_str());
  ASSERT_FALSE(request->is_bad_request);
}
