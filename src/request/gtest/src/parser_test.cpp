#include <gtest/gtest.h>

#include "HttpRequest.hpp"
#include "HttpRequestParser.hpp"
#include "ServerConfig.hpp"
#include "test_helper.hpp"

// requestの内容をファイルから読み込んでいるが、直接ベタ打ちした方がよかったい気がする。

class HttpRequestParserTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    config.port = 4242;
    config.host = "42tokyo";
    config.server.push_back("nop");
    config.server.push_back("cluster");
    // error_pages_は省略
    config.root = "www/html";
    config.client_body_size_limit = 65536;
    // locations_は省略
  }
  HttpRequestParser parser;
  ServerConfig config;
};

TEST_F(HttpRequestParserTest, StoreRequestline) {
  std::string file_content = readFile("gtest/request/simple_get.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkRequestline("GET", "/", "HTTP/1.1", request);
  ASSERT_EQ(HttpStatus::OK, request->response_status_code);
}

TEST_F(HttpRequestParserTest, StoreHeaderFieldWithCurl) {
  std::string file_content = readFile("gtest/request/curl.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkRequestline("GET", "/", "HTTP/1.1", request);
  checkHeaderField("Host", "localhost:4242", request->name_value_map);
  checkHeaderField("User-Agent", "curl/7.68.0", request->name_value_map);
  checkHeaderField("Accept", "*/*", request->name_value_map);
  ASSERT_EQ(3, request->name_value_map.size());
  ASSERT_EQ(HttpStatus::OK, request->response_status_code);
}

TEST_F(HttpRequestParserTest, StoreHeaderFieldWithChrome) {
  std::string file_content = readFile("gtest/request/chrome.crlf");
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
  ASSERT_EQ(HttpStatus::OK, request->response_status_code);
}

TEST_F(HttpRequestParserTest, StoreOneLineToBody) {
  std::string file_content = readFile("gtest/request/body_one_line.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkRequestline("POST", "/", "HTTP/1.1", request);
  checkHeaderField("Host", "localhost:80", request->name_value_map);
  checkBody("name=hoge&comment=hoge\n", request->body);
  ASSERT_EQ(1, request->name_value_map.size());
  ASSERT_EQ(HttpStatus::OK, request->response_status_code);
}

TEST_F(HttpRequestParserTest, StoreBodyMultiLinesToBody) {
  std::string file_content = readFile("gtest/request/body_multi_lines.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkRequestline("POST", "/", "HTTP/1.1", request);
  checkHeaderField("Host", "localhost:80", request->name_value_map);
  checkBody("1stline\n2ndline\n3rdline\n", request->body);
  ASSERT_EQ(1, request->name_value_map.size());
  ASSERT_EQ(HttpStatus::OK, request->response_status_code);
}

TEST_F(HttpRequestParserTest, StoreJsonToBody) {
  std::string file_content = readFile("gtest/request/body_json.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkRequestline("POST", "/", "HTTP/1.1", request);
  checkHeaderField("Host", "admin", request->name_value_map);
  checkHeaderField("User-Agent", "send_response.sh", request->name_value_map);
  checkHeaderField("Content-Type", "application/json", request->name_value_map);
  checkBody("{\n\t\"asa-gohan\":\"misosiru\",\n\t\"oyatsu\":\"karl\"\n}\n",
            request->body);
  ASSERT_EQ(3, request->name_value_map.size());
  ASSERT_EQ(HttpStatus::OK, request->response_status_code);
}

TEST_F(HttpRequestParserTest, StoreServerConfig) {
  std::string file_content = readFile("gtest/request/simple_get.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkRequestline("GET", "/", "HTTP/1.1", request);
  checkBody("", request->body);
  ASSERT_EQ(2, request->name_value_map.size());
  ASSERT_EQ(4242, request->server_config.port);
  ASSERT_EQ("42tokyo", request->server_config.host);
  ASSERT_EQ("nop", request->server_config.server[0]);
  ASSERT_EQ("cluster", request->server_config.server[1]);
  ASSERT_EQ("www/html", request->server_config.root);
  ASSERT_EQ(65536, request->server_config.client_body_size_limit);
  ASSERT_EQ(HttpStatus::OK, request->response_status_code);
}

TEST_F(HttpRequestParserTest, InvalidHttpVersion) {
  std::string file_content =
      readFile("gtest/request/invalid_http_version.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, request->response_status_code);
}

TEST_F(HttpRequestParserTest, NoRequestLine) {
  std::string file_content = readFile("gtest/request/no_request_line.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, request->response_status_code);
}

TEST_F(HttpRequestParserTest, NoMethod) {
  std::string file_content = readFile("gtest/request/no_method.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, request->response_status_code);
}

TEST_F(HttpRequestParserTest, NoUri) {
  std::string file_content = readFile("gtest/request/no_uri.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, request->response_status_code);
}

TEST_F(HttpRequestParserTest, NoHttpVersion) {
  std::string file_content = readFile("gtest/request/no_http_version.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, request->response_status_code);
}

TEST_F(HttpRequestParserTest, InvalidProtocol) {
  std::string file_content = readFile("gtest/request/invalid_protocol.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, request->response_status_code);
}

TEST_F(HttpRequestParserTest, VersionHasNoSlash) {
  std::string file_content =
      readFile("gtest/request/version_has_no_slash.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, request->response_status_code);
}

TEST_F(HttpRequestParserTest, VersionHasNoPeriod) {
  std::string file_content =
      readFile("gtest/request/version_has_no_period.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, request->response_status_code);
}

TEST_F(HttpRequestParserTest, VersionHasOtherThanDigit) {
  std::string file_content =
      readFile("gtest/request/version_has_other_than_digit.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, request->response_status_code);
}

TEST_F(HttpRequestParserTest, VersionIsNotSupported) {
  std::string file_content =
      readFile("gtest/request/version_is_not_supported.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_EQ(HttpStatus::HTTP_VERSION_NOT_SUPPORTED,
            request->response_status_code);
}

TEST_F(HttpRequestParserTest, HeaderHasNoColon) {
  std::string file_content = readFile("gtest/request/header_has_no_colon.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkRequestline("GET", "/", "HTTP/1.1", request);
  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, request->response_status_code);
}

TEST_F(HttpRequestParserTest, HeaderHasSpaceBeforeColon) {
  std::string file_content =
      readFile("gtest/request/header_has_space_before_colon.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkRequestline("GET", "/", "HTTP/1.1", request);
  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, request->response_status_code);
}

TEST_F(HttpRequestParserTest, HeaderHasNoFieldName) {
  std::string file_content =
      readFile("gtest/request/header_has_no_field_name.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkRequestline("GET", "/", "HTTP/1.1", request);
  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, request->response_status_code);
}

TEST_F(HttpRequestParserTest, CheckFieldValueIsTrimmedByWS) {
  std::string file_content =
      readFile("gtest/request/field_value_is_trimmed.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkRequestline("GET", "/", "HTTP/1.1", request);
  checkBody("", request->body);
  checkHeaderField("Host", "admin", request->name_value_map);
  ASSERT_EQ(2, request->name_value_map.size());
  ASSERT_EQ(HttpStatus::OK, request->response_status_code);
}

TEST_F(HttpRequestParserTest, FieldValueHasOnlyWS) {
  std::string file_content =
      readFile("gtest/request/field_value_has_only_WS.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkRequestline("GET", "/", "HTTP/1.1", request);
  checkBody("", request->body);
  checkHeaderField("Host", "admin", request->name_value_map);
  ASSERT_EQ(2, request->name_value_map.size());
  ASSERT_EQ(HttpStatus::OK, request->response_status_code);
}

TEST_F(HttpRequestParserTest, FieldNameHasOnlyWS) {
  std::string file_content =
      readFile("gtest/request/field_name_has_only_WS.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkRequestline("GET", "/", "HTTP/1.1", request);
  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, request->response_status_code);
}

TEST_F(HttpRequestParserTest, FieldNameHasInvalidChar) {
  std::string file_content =
      readFile("gtest/request/field_name_has_invalid_char.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkRequestline("GET", "/", "HTTP/1.1", request);
  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, request->response_status_code);
}

TEST_F(HttpRequestParserTest, FieldNameLastIsInvalidChar) {
  std::string file_content =
      readFile("gtest/request/field_name_last_is_invalid_char.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkRequestline("GET", "/", "HTTP/1.1", request);
  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, request->response_status_code);
}

TEST_F(HttpRequestParserTest, NoHost) {
  std::string file_content = readFile("gtest/request/no_host.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkRequestline("GET", "/", "HTTP/1.1", request);
  checkBody("", request->body);
  ASSERT_EQ(0, request->name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, request->response_status_code);
}

TEST_F(HttpRequestParserTest, SameHeaderFieldName) {
  std::string file_content =
      readFile("gtest/request/same_header_field_name.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  checkRequestline("GET", "/", "HTTP/1.1", request);
  checkBody("", request->body);
  checkHeaderField("Host", "admin", request->name_value_map);
  checkHeaderField("user-agent", "agent1,agent2", request->name_value_map);
  ASSERT_EQ(2, request->name_value_map.size());
  ASSERT_EQ(HttpStatus::OK, request->response_status_code);
}
