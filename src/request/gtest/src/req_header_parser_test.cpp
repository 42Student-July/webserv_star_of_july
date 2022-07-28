#include <gtest/gtest.h>

#include "HttpRequest.hpp"
#include "HttpRequestParser.hpp"
#include "ServerConfig.hpp"
#include "test_helper.hpp"

// requestの内容をファイルから読み込んでいるが、直接ベタ打ちした方がよかったい気がする。

class RequestHeaderParserTest : public ::testing::Test {
 protected:
  virtual void SetUp() {}

  void testException(const std::string& file_name,
                     const std::string& status_code, const char* error_msg) {
    ASSERT_THROW(buildRequestHeader(file_dir + file_name),
                 HttpParser::ParseErrorExeption);
    try {
      RequestHeader header = buildRequestHeader(file_dir + file_name);
    } catch (const HttpParser::ParseErrorExeption& e) {
      ASSERT_EQ(status_code, e.getErrorStatus());
      ASSERT_STREQ(error_msg, e.what());
    }
  }
};

TEST_F(RequestHeaderParserTest, StoreRequestline) {
  std::string file_name = "simple_get.crlf";
  RequestHeader header = buildRequestHeader(file_dir + file_name);

  checkRequestline("GET", "/", "HTTP/1.1", header.request_line);
}

TEST_F(RequestHeaderParserTest, StoreHeaderFieldWithCurl) {
  std::string file_name = "curl.crlf";
  RequestHeader header = buildRequestHeader(file_dir + file_name);

  checkRequestline("GET", "/", "HTTP/1.1", header.request_line);
  checkHeaderField("host", "localhost:4242", header.name_value_map);
  checkHeaderField("user-agent", "curl/7.68.0", header.name_value_map);
  checkHeaderField("accept", "*/*", header.name_value_map);
  ASSERT_EQ(3, header.name_value_map.size());
}

TEST_F(RequestHeaderParserTest, StoreHeaderFieldWithChrome) {
  std::string file_name = "chrome.crlf";
  RequestHeader header = buildRequestHeader(file_dir + file_name);

  checkRequestline("GET", "/", "HTTP/1.1", header.request_line);
  checkHeaderField("host", "localhost:4242", header.name_value_map);
  checkHeaderField("connection", "keep-alive", header.name_value_map);
  checkHeaderField("cache-control", "max-age=0", header.name_value_map);
  checkHeaderField("sec-ch-ua",
                   "\".Not/A)Brand\";v=\"99\", \"Google "
                   "Chrome\";v=\"103\", \"Chromium\";v=\"103\"",
                   header.name_value_map);
  checkHeaderField("sec-ch-ua-mobile", "?0", header.name_value_map);
  checkHeaderField("sec-ch-ua-platform", "\"Linux\"", header.name_value_map);
  checkHeaderField("upgrade-insecure-requests", "1", header.name_value_map);
  checkHeaderField("user-agent",
                   "Mozilla/5.0 (X11; Linux x86_64) "
                   "AppleWebKit/537.36 (KHTML, "
                   "like Gecko) Chrome/103.0.0.0 Safari/537.36",
                   header.name_value_map);
  checkHeaderField(
      "accept",
      "text/html,application/xhtml+xml,application/xml;q=0.9,image/"
      "avif,image/"
      "webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9",
      header.name_value_map);
  checkHeaderField("sec-fetch-site", "none", header.name_value_map);
  checkHeaderField("sec-fetch-mode", "navigate", header.name_value_map);
  checkHeaderField("sec-fetch-dest", "document", header.name_value_map);
  checkHeaderField("accept-encoding", "gzip, deflate, br",
                   header.name_value_map);
  checkHeaderField("accept-language", "ja,en;q=0.9,zh-CN;q=0.8,zh;q=0.7",
                   header.name_value_map);
  ASSERT_EQ(15, header.name_value_map.size());
}

// テスト名（テストファイル名とエラーメッセージが対応してないことがある
// 検知される場所によるので気にしない。
TEST_F(RequestHeaderParserTest, InvalidHttpVersion) {
  std::string file_name = "invalid_http_version.crlf";
  testException(file_name, HttpStatus::BAD_REQUEST, "HttpVersion length error");
}

TEST_F(RequestHeaderParserTest, EmptyFile) {
  std::string file_name = "empty.crlf";
  testException(file_name, HttpStatus::BAD_REQUEST, "getLine() error");
}

TEST_F(RequestHeaderParserTest, NoRequestLine) {
  std::string file_name = "no_request_line.crlf";
  testException(file_name, HttpStatus::BAD_REQUEST, "Invalid Method");
}

TEST_F(RequestHeaderParserTest, NoMethod) {
  std::string file_name = "no_method.crlf";
  testException(file_name, HttpStatus::BAD_REQUEST, "Invalid Method");
}

TEST_F(RequestHeaderParserTest, NoUri) {
  std::string file_name = "no_uri.crlf";
  testException(file_name, HttpStatus::BAD_REQUEST, "HttpVersion length error");
}

TEST_F(RequestHeaderParserTest, NoHttpVersion) {
  std::string file_name = "no_http_version.crlf";
  testException(file_name, HttpStatus::BAD_REQUEST, "HttpVersion length error");
}

TEST_F(RequestHeaderParserTest, InvalidMethod) {
  std::string file_name = "invalid_method.crlf";
  testException(file_name, HttpStatus::BAD_REQUEST, "Invalid Method");
}

TEST_F(RequestHeaderParserTest, InvalidProtocol) {
  std::string file_name = "invalid_protocol.crlf";
  testException(file_name, HttpStatus::BAD_REQUEST, "Invalid Protocol");
}

TEST_F(RequestHeaderParserTest, VersionHasNoSlash) {
  std::string file_name = "version_has_no_slash.crlf";
  testException(file_name, HttpStatus::BAD_REQUEST, "Invalid Protocol");
}

TEST_F(RequestHeaderParserTest, VersionHasNoPeriod) {
  std::string file_name = "version_has_no_period.crlf";
  testException(file_name, HttpStatus::BAD_REQUEST, ". is not found");
}

TEST_F(RequestHeaderParserTest, VersionHasOtherThanDigit) {
  std::string file_name = "version_has_other_than_digit.crlf";
  testException(file_name, HttpStatus::BAD_REQUEST,
                "version has other than digit");
}

TEST_F(RequestHeaderParserTest, VersionIsNotSupported) {
  std::string file_name = "version_is_not_supported.crlf";
  testException(file_name, HttpStatus::HTTP_VERSION_NOT_SUPPORTED,
                "version is not supported");
}

TEST_F(RequestHeaderParserTest, HeaderHasNoColon) {
  std::string file_name = "header_has_no_colon.crlf";
  testException(file_name, HttpStatus::BAD_REQUEST, "header has no colon");
}

TEST_F(RequestHeaderParserTest, HeaderHasSpaceBeforeColon) {
  std::string file_name = "header_has_space_before_colon.crlf";
  testException(file_name, HttpStatus::BAD_REQUEST,
                "header has space before colon");
}

TEST_F(RequestHeaderParserTest, HeaderHasNoFieldName) {
  std::string file_name = "header_has_no_field_name.crlf";
  testException(file_name, HttpStatus::BAD_REQUEST, "header has no field name");
}

TEST_F(RequestHeaderParserTest, CheckFieldValueIsTrimmedByWS) {
  std::string file_name = "field_value_is_trimmed.crlf";
  RequestHeader header = buildRequestHeader(file_dir + file_name);

  checkRequestline("GET", "/", "HTTP/1.1", header.request_line);
  checkHeaderField("host", "admin", header.name_value_map);
  ASSERT_EQ(2, header.name_value_map.size());
}

TEST_F(RequestHeaderParserTest, FieldValueHasOnlyWS) {
  std::string file_name = "field_value_has_only_WS.crlf";
  RequestHeader header = buildRequestHeader(file_dir + file_name);

  checkRequestline("GET", "/", "HTTP/1.1", header.request_line);
  ASSERT_EQ(2, header.name_value_map.size());
}

TEST_F(RequestHeaderParserTest, FieldNameHasOnlyWS) {
  std::string file_name = "field_name_has_only_WS.crlf";
  testException(file_name, HttpStatus::BAD_REQUEST,
                "header has space before colon");
}

TEST_F(RequestHeaderParserTest, FieldNameHasInvalidChar) {
  std::string file_name = "field_name_has_invalid_char.crlf";
  testException(file_name, HttpStatus::BAD_REQUEST,
                "field_name is not header token");
}

TEST_F(RequestHeaderParserTest, FieldNameLastIsInvalidChar) {
  std::string file_name = "field_name_last_is_invalid_char.crlf";
  testException(file_name, HttpStatus::BAD_REQUEST,
                "field_name is not header token");
}

TEST_F(RequestHeaderParserTest, NoHost) {
  std::string file_name = "no_host.crlf";
  testException(file_name, HttpStatus::BAD_REQUEST, "no host");
}

TEST_F(RequestHeaderParserTest, NoHostEmpty) {
  std::string file_name = "no_host_empty.crlf";
  testException(file_name, HttpStatus::BAD_REQUEST, "no host");
}

TEST_F(RequestHeaderParserTest, TwoHost) {
  std::string file_name = "two_host.crlf";
  testException(file_name, HttpStatus::BAD_REQUEST, "header has two Host");
}

TEST_F(RequestHeaderParserTest, SameHeaderFieldName) {
  std::string file_name = "same_header_field_name.crlf";
  RequestHeader header = buildRequestHeader(file_dir + file_name);

  checkRequestline("GET", "/", "HTTP/1.1", header.request_line);
  checkHeaderField("host", "admin", header.name_value_map);
  checkHeaderField("user-agent", "agent1,agent2", header.name_value_map);
  ASSERT_EQ(2, header.name_value_map.size());
}

TEST_F(RequestHeaderParserTest, FieldNameCaseInsesitive) {
  std::string file_name = "field_name_case_insensitive.crlf";
  RequestHeader header = buildRequestHeader(file_dir + file_name);

  checkRequestline("GET", "/", "HTTP/1.1", header.request_line);
  checkHeaderField("host", "admin", header.name_value_map);
  checkHeaderField("user-agent", "agent1", header.name_value_map);
  checkHeaderField("accept", "text/html", header.name_value_map);
  ASSERT_EQ(3, header.name_value_map.size());
}

TEST_F(RequestHeaderParserTest, NoCrRequestLine) {
  std::string file_name = "no_cr_request_line.crlf";
  testException(file_name, HttpStatus::BAD_REQUEST, "HttpVersion length error");
}

TEST_F(RequestHeaderParserTest, NoCrHeaderField) {
  std::string file_name = "no_cr_header_field.crlf";
  testException(file_name, HttpStatus::BAD_REQUEST, "getLine() error");
}

TEST_F(RequestHeaderParserTest, NoCrHeaderEnd) {
  std::string file_name = "no_cr_header_end.crlf";
  testException(file_name, HttpStatus::BAD_REQUEST, "getLine() error");
}
