#include <gtest/gtest.h>

#include "HttpRequest.hpp"
#include "HttpRequestParser.hpp"
#include "ServerConfig.hpp"
#include "test_helper.hpp"

TEST(HttpParserTest, StoreRequestline) {
  HttpRequestParser parser;
  ServerConfig config = initServerCongig();
  std::string file_content = readFile("request/simple_get.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkRequestline("GET", "/", "HTTP/1.1", request);
  ASSERT_FALSE(request->is_bad_request);
}

TEST(HttpParserTest, StoreHeaderFieldWithCurl) {
  HttpRequestParser parser;
  ServerConfig config = initServerCongig();
  std::string file_content = readFile("request/curl.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkRequestline("GET", "/", "HTTP/1.1", request);
  checkHeaderField("Host", "localhost:4242", request->name_value_map);
  checkHeaderField("User-Agent", "curl/7.68.0", request->name_value_map);
  checkHeaderField("Accept", "*/*", request->name_value_map);
  ASSERT_EQ(3, request->name_value_map.size());
  ASSERT_FALSE(request->is_bad_request);
}

TEST(HttpParserTest, StoreHeaderFieldWithChrome) {
  HttpRequestParser parser;
  ServerConfig config = initServerCongig();
  std::string file_content = readFile("request/chrome.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkRequestline("GET", "/", "HTTP/1.1", request);
  checkHeaderField("Host", "localhost:4242", request->name_value_map);
  checkHeaderField("Connection", "keep-alive", request->name_value_map);
  checkHeaderField("Cache-Control", "max-age=0", request->name_value_map);
  checkHeaderField("sec-ch-ua",
                   "\".Not/A)Brand\";v=\"99\", \"Google "
                   "Chrome\";v=\"103\", \"Chromium\";v=\"103\"",
                   request->name_value_map);
  checkHeaderField("sec-ch-ua-mobile", "?0", request->name_value_map);
  checkHeaderField("sec-ch-ua-platform", "\"Linux\"", request->name_value_map);
  checkHeaderField("Upgrade-Insecure-Requests", "1", request->name_value_map);
  checkHeaderField("User-Agent",
                   "Mozilla/5.0 (X11; Linux x86_64) "
                   "AppleWebKit/537.36 (KHTML, "
                   "like Gecko) Chrome/103.0.0.0 Safari/537.36",
                   request->name_value_map);
  checkHeaderField(
      "Accept",
      "text/html,application/xhtml+xml,application/xml;q=0.9,image/"
      "avif,image/"
      "webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9",
      request->name_value_map);
  checkHeaderField("Sec-Fetch-Site", "none", request->name_value_map);
  checkHeaderField("Sec-Fetch-Mode", "navigate", request->name_value_map);
  checkHeaderField("Sec-Fetch-Dest", "document", request->name_value_map);
  checkHeaderField("Accept-Encoding", "gzip, deflate, br",
                   request->name_value_map);
  checkHeaderField("Accept-Language", "ja,en;q=0.9,zh-CN;q=0.8,zh;q=0.7",
                   request->name_value_map);
  ASSERT_EQ(15, request->name_value_map.size());
  ASSERT_FALSE(request->is_bad_request);
}

TEST(HttpParserTest, StoreOneLineToBody) {
  HttpRequestParser parser;
  ServerConfig config = initServerCongig();
  std::string file_content = readFile("request/body_one_line.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkRequestline("POST", "/", "HTTP/1.1", request);
  checkHeaderField("Host", "localhost:80", request->name_value_map);
  checkBody("name=hoge&comment=hoge\n", request->body);
  ASSERT_EQ(1, request->name_value_map.size());
  ASSERT_FALSE(request->is_bad_request);
}

TEST(HttpParserTest, StoreBodyMultiLinesToBody) {
  HttpRequestParser parser;
  ServerConfig config = initServerCongig();
  std::string file_content = readFile("request/body_multi_lines.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkRequestline("POST", "/", "HTTP/1.1", request);
  checkHeaderField("Host", "localhost:80", request->name_value_map);
  checkBody("1stline\n2ndline\n3rdline\n", request->body);
  ASSERT_EQ(1, request->name_value_map.size());
  ASSERT_FALSE(request->is_bad_request);
}

TEST(HttpParserTest, StoreJsonToBody) {
  HttpRequestParser parser;
  ServerConfig config = initServerCongig();
  std::string file_content = readFile("request/body_json.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkRequestline("POST", "/", "HTTP/1.1", request);
  checkHeaderField("Host", "admin", request->name_value_map);
  checkHeaderField("User-Agent", "send_response.sh", request->name_value_map);
  checkHeaderField("Content-Type", "application/json", request->name_value_map);
  checkBody("{\n\t\"asa-gohan\":\"misosiru\",\n\t\"oyatsu\":\"karl\"\n}\n",
            request->body);
  ASSERT_EQ(3, request->name_value_map.size());
  ASSERT_FALSE(request->is_bad_request);
}

TEST(HttpParserTest, StoreServerConfig) {
  HttpRequestParser parser;
  ServerConfig config = initServerCongig();
  std::string file_content = readFile("request/simple_get.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkRequestline("GET", "/", "HTTP/1.1", request);
  checkBody("", request->body);
  ASSERT_EQ(2, request->name_value_map.size());
  ASSERT_EQ(4242, request->server_config.port);
  compareString("42tokyo", request->server_config.host);
  compareString("nop", request->server_config.server[0]);
  compareString("cluster", request->server_config.server[1]);
  compareString("www/html", request->server_config.root);
  ASSERT_EQ(65536, request->server_config.client_body_size_limit);
  ASSERT_FALSE(request->is_bad_request);
}

TEST(HttpParserTest, InvalidHttpVersion) {
  HttpRequestParser parser;
  ServerConfig config = initServerCongig();
  std::string file_content = readFile("request/invalid_http_version.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_TRUE(request->is_bad_request);
}

TEST(HttpParserTest, NoRequestLine) {
  HttpRequestParser parser;
  ServerConfig config = initServerCongig();
  std::string file_content = readFile("request/no_request_line.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_TRUE(request->is_bad_request);
  compareString("400", request->status);
}

// parserでは200:OK
TEST(HttpParserTest, NoMethod) {
  HttpRequestParser parser;
  ServerConfig config = initServerCongig();
  std::string file_content = readFile("request/no_method.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_TRUE(request->is_bad_request);
  compareString("400", request->status);
}

// parserでは200:OK
TEST(HttpParserTest, NoUri) {
  HttpRequestParser parser;
  ServerConfig config = initServerCongig();
  std::string file_content = readFile("request/no_uri.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_TRUE(request->is_bad_request);
  compareString("400", request->status);
}

TEST(HttpParserTest, NoHttpVersion) {
  HttpRequestParser parser;
  ServerConfig config = initServerCongig();
  std::string file_content = readFile("request/no_http_version.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_TRUE(request->is_bad_request);
  compareString("400", request->status);
}

TEST(HttpParserTest, InvalidProtocol) {
  HttpRequestParser parser;
  ServerConfig config = initServerCongig();
  std::string file_content = readFile("request/invalid_protocol.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_TRUE(request->is_bad_request);
  compareString("400", request->status);
}

TEST(HttpParserTest, VersionHasNoSlash) {
  HttpRequestParser parser;
  ServerConfig config = initServerCongig();
  std::string file_content = readFile("request/version_has_no_slash.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_TRUE(request->is_bad_request);
  compareString("400", request->status);
}

TEST(HttpParserTest, VersionHasNoPeriod) {
  HttpRequestParser parser;
  ServerConfig config = initServerCongig();
  std::string file_content = readFile("request/version_has_no_period.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_TRUE(request->is_bad_request);
  compareString("400", request->status);
}

TEST(HttpParserTest, VersionHasOtherThanDigit) {
  HttpRequestParser parser;
  ServerConfig config = initServerCongig();
  std::string file_content =
      readFile("request/version_has_other_than_digit.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_TRUE(request->is_bad_request);
  compareString("400", request->status);
}

TEST(HttpParserTest, VersionIsNotSupported) {
  HttpRequestParser parser;
  ServerConfig config = initServerCongig();
  std::string file_content = readFile("request/version_is_not_supported.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_TRUE(request->is_bad_request);
  compareString("505", request->status);
}
