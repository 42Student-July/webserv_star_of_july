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

TEST(HttpParserTest, Requestline) {
  HttpRequestParser parser;
  std::string file_content = readFile("gtest/request/simple_get.crlf");

  HttpRequestDTO *request = parser.parse(file_content.c_str());
  ASSERT_STREQ("GET", request->method.c_str());
  ASSERT_STREQ("/", request->uri.c_str());
  ASSERT_STREQ("HTTP/1.1", request->version.c_str());
}

TEST(HttpParserTest, StoreHeaderFieldWithCurl) {
  HttpRequestParser parser;
  std::string file_content = readFile("gtest/request/curl.crlf");

  HttpRequestDTO *request = parser.parse(file_content.c_str());
  HttpRequestParser::HeaderFieldMap headerfields = parser.getHeaderFieldMap();
  ASSERT_STREQ("localhost:4242", headerfields["Host"].c_str());
  ASSERT_STREQ("curl/7.68.0", headerfields["User-Agent"].c_str());
  ASSERT_STREQ("*/*", headerfields["Accept"].c_str());
  ASSERT_EQ(3, headerfields.size());
}

TEST(HttpParserTest, StoreHeaderFieldWithChrome) {
  HttpRequestParser parser;
  std::string file_content = readFile("gtest/request/chrome.crlf");

  HttpRequestDTO *request = parser.parse(file_content.c_str());
  HttpRequestParser::HeaderFieldMap headerfields = parser.getHeaderFieldMap();
  ASSERT_STREQ("localhost:4242", headerfields["Host"].c_str());
  ASSERT_STREQ("keep-alive", headerfields["Connection"].c_str());
  ASSERT_STREQ("max-age=0", headerfields["Cache-Control"].c_str());
  ASSERT_STREQ(
      "\".Not/A)Brand\";v=\"99\", \"Google Chrome\";v=\"103\", "
      "\"Chromium\";v=\"103\"",
      headerfields["sec-ch-ua"].c_str());
  ASSERT_STREQ("?0", headerfields["sec-ch-ua-mobile"].c_str());
  ASSERT_STREQ("\"Linux\"", headerfields["sec-ch-ua-platform"].c_str());
  ASSERT_STREQ("1", headerfields["Upgrade-Insecure-Requests"].c_str());
  ASSERT_STREQ(
      "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) "
      "Chrome/103.0.0.0 Safari/537.36",
      headerfields["User-Agent"].c_str());
  ASSERT_STREQ(
      "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/"
      "webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9",
      headerfields["Accept"].c_str());
  ASSERT_STREQ("none", headerfields["Sec-Fetch-Site"].c_str());
  ASSERT_STREQ("navigate", headerfields["Sec-Fetch-Mode"].c_str());
  ASSERT_STREQ("document", headerfields["Sec-Fetch-Dest"].c_str());
  ASSERT_STREQ("gzip, deflate, br", headerfields["Accept-Encoding"].c_str());
  ASSERT_STREQ("ja,en;q=0.9,zh-CN;q=0.8,zh;q=0.7",
               headerfields["Accept-Language"].c_str());
  ASSERT_EQ(15, headerfields.size());
}
