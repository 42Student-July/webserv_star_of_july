#include <gtest/gtest.h>

#include <fstream>

#include "HttpRequest.hpp"
#include "HttpRequestParser.hpp"

// ヘルパー関数
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

void checkRequestline(HttpRequest *request, const std::string &method,
                      const std::string &uri, const std::string &version) {
  ASSERT_STREQ(method.c_str(), request->method.c_str());
  ASSERT_STREQ(uri.c_str(), request->uri.c_str());
  ASSERT_STREQ(version.c_str(), request->version.c_str());
}

void checkHeaderField(HttpRequest *request, const std::string &name,
                      const std::string &value) {
  ASSERT_STREQ(value.c_str(), request->name_value_map[name].c_str());
}

void checkBody(HttpRequest *request, const std::string &body) {
  ASSERT_STREQ(body.c_str(), request->body.c_str());
}

// ここからテスト
TEST(HttpParserTest, StoreRequestline) {
  HttpRequestParser parser;
  std::string file_content = readFile("request/simple_get.crlf");
  HttpRequest *request = parser.parse(file_content.c_str());

  checkRequestline(request, "GET", "/", "HTTP/1.1");
}

TEST(HttpParserTest, StoreHeaderFieldWithCurl) {
  HttpRequestParser parser;
  std::string file_content = readFile("request/curl.crlf");
  HttpRequest *request = parser.parse(file_content.c_str());

  checkRequestline(request, "GET", "/", "HTTP/1.1");
  checkHeaderField(request, "Host", "localhost:4242");
  checkHeaderField(request, "User-Agent", "curl/7.68.0");
  checkHeaderField(request, "Accept", "*/*");
  ASSERT_EQ(3, request->name_value_map.size());
}

TEST(HttpParserTest, StoreHeaderFieldWithChrome) {
  HttpRequestParser parser;
  std::string file_content = readFile("request/chrome.crlf");
  HttpRequest *request = parser.parse(file_content.c_str());

  checkRequestline(request, "GET", "/", "HTTP/1.1");
  checkHeaderField(request, "Host", "localhost:4242");
  checkHeaderField(request, "Connection", "keep-alive");
  checkHeaderField(request, "Cache-Control", "max-age=0");
  checkHeaderField(request, "Cache-Control", "max-age=0");
  checkHeaderField(request, "Cache-Control", "max-age=0");
  checkHeaderField(request, "Cache-Control", "max-age=0");
  checkHeaderField(request, "sec-ch-ua",
                   "\".Not/A)Brand\";v=\"99\", \"Google "
                   "Chrome\";v=\"103\", \"Chromium\";v=\"103\"");
  checkHeaderField(request, "sec-ch-ua-mobile", "?0");
  checkHeaderField(request, "sec-ch-ua-platform", "\"Linux\"");
  checkHeaderField(request, "Upgrade-Insecure-Requests", "1");
  checkHeaderField(request, "User-Agent",
                   "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, "
                   "like Gecko) Chrome/103.0.0.0 Safari/537.36");
  checkHeaderField(
      request, "Accept",
      "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/"
      "webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9");
  checkHeaderField(request, "Sec-Fetch-Site", "none");
  checkHeaderField(request, "Sec-Fetch-Mode", "navigate");
  checkHeaderField(request, "Sec-Fetch-Dest", "document");
  checkHeaderField(request, "Accept-Encoding", "gzip, deflate, br");
  checkHeaderField(request, "Accept-Language",
                   "ja,en;q=0.9,zh-CN;q=0.8,zh;q=0.7");
  ASSERT_EQ(15, request->name_value_map.size());
}

TEST(HttpParserTest, StoreOneLineToBody) {
  HttpRequestParser parser;
  std::string file_content = readFile("request/body_one_line.crlf");
  HttpRequest *request = parser.parse(file_content.c_str());

  checkRequestline(request, "POST", "/", "HTTP/1.1");
  checkHeaderField(request, "Host", "localhost:80");
  checkBody(request, "name=hoge&comment=hoge\n");
  ASSERT_EQ(1, request->name_value_map.size());
}

TEST(HttpParserTest, StoreBodyMultiLinesToBody) {
  HttpRequestParser parser;
  std::string file_content = readFile("request/body_multi_lines.crlf");
  HttpRequest *request = parser.parse(file_content.c_str());

  checkRequestline(request, "POST", "/", "HTTP/1.1");
  checkHeaderField(request, "Host", "localhost:80");
  checkBody(request, "1stline\n2ndline\n3rdline\n");
  ASSERT_EQ(1, request->name_value_map.size());
}

TEST(HttpParserTest, StoreJsonToBody) {
  HttpRequestParser parser;
  std::string file_content = readFile("request/body_json.crlf");
  HttpRequest *request = parser.parse(file_content.c_str());

  checkRequestline(request, "POST", "/", "HTTP/1.1");
  checkHeaderField(request, "Host", "admin");
  checkHeaderField(request, "User-Agent", "send_response.sh");
  checkHeaderField(request, "Content-Type", "application/json");
  checkBody(request,
            "{\n\t\"asa-gohan\":\"misosiru\",\n\t\"oyatsu\":\"karl\"\n}\n");
  ASSERT_EQ(3, request->name_value_map.size());
}
