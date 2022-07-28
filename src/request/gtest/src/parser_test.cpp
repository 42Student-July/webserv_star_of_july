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
  ServerConfig config;
};

TEST_F(HttpRequestParserTest, StoreRequestline) {
  std::string file_name = "simple_get.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkRequestline("GET", "/", "HTTP/1.1", req);
  ASSERT_EQ(HttpStatus::OK, req->response_status_code);
}

TEST_F(HttpRequestParserTest, StoreHeaderFieldWithCurl) {
  std::string file_name = "curl.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkRequestline("GET", "/", "HTTP/1.1", req);
  checkHeaderField("host", "localhost:4242", req->header.name_value_map);
  checkHeaderField("user-agent", "curl/7.68.0", req->header.name_value_map);
  checkHeaderField("accept", "*/*", req->header.name_value_map);
  ASSERT_EQ(3, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::OK, req->response_status_code);
}

TEST_F(HttpRequestParserTest, StoreHeaderFieldWithChrome) {
  std::string file_name = "chrome.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkRequestline("GET", "/", "HTTP/1.1", req);
  checkHeaderField("host", "localhost:4242", req->header.name_value_map);
  checkHeaderField("connection", "keep-alive", req->header.name_value_map);
  checkHeaderField("cache-control", "max-age=0", req->header.name_value_map);
  checkHeaderField("sec-ch-ua",
                   "\".Not/A)Brand\";v=\"99\", \"Google "
                   "Chrome\";v=\"103\", \"Chromium\";v=\"103\"",
                   req->header.name_value_map);
  checkHeaderField("sec-ch-ua-mobile", "?0", req->header.name_value_map);
  checkHeaderField("sec-ch-ua-platform", "\"Linux\"",
                   req->header.name_value_map);
  checkHeaderField("upgrade-insecure-requests", "1",
                   req->header.name_value_map);
  checkHeaderField("user-agent",
                   "Mozilla/5.0 (X11; Linux x86_64) "
                   "AppleWebKit/537.36 (KHTML, "
                   "like Gecko) Chrome/103.0.0.0 Safari/537.36",
                   req->header.name_value_map);
  checkHeaderField(
      "accept",
      "text/html,application/xhtml+xml,application/xml;q=0.9,image/"
      "avif,image/"
      "webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9",
      req->header.name_value_map);
  checkHeaderField("sec-fetch-site", "none", req->header.name_value_map);
  checkHeaderField("sec-fetch-mode", "navigate", req->header.name_value_map);
  checkHeaderField("sec-fetch-dest", "document", req->header.name_value_map);
  checkHeaderField("accept-encoding", "gzip, deflate, br",
                   req->header.name_value_map);
  checkHeaderField("accept-language", "ja,en;q=0.9,zh-CN;q=0.8,zh;q=0.7",
                   req->header.name_value_map);
  ASSERT_EQ(15, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::OK, req->response_status_code);
}

TEST_F(HttpRequestParserTest, StoreOneLineToBody) {
  std::string file_name = "body_one_line.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkRequestline("POST", "/", "HTTP/1.1", req);
  checkHeaderField("host", "localhost:80", req->header.name_value_map);
  checkHeaderField("content-length", "23", req->header.name_value_map);
  checkBody("name=hoge&comment=hoge\n", req->body);
  ASSERT_EQ(2, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::OK, req->response_status_code);
}

TEST_F(HttpRequestParserTest, StoreMultiLinesToBody) {
  std::string file_name = "body_multi_lines.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkRequestline("POST", "/", "HTTP/1.1", req);
  checkHeaderField("host", "localhost:80", req->header.name_value_map);
  checkHeaderField("content-length", "24", req->header.name_value_map);
  checkBody("1stline\n2ndline\n3rdline\n", req->body);
  ASSERT_EQ(2, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::OK, req->response_status_code);
}

TEST_F(HttpRequestParserTest, StoreJsonToBody) {
  std::string file_name = "body_json.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkRequestline("POST", "/", "HTTP/1.1", req);
  checkHeaderField("host", "localhost:80", req->header.name_value_map);
  checkHeaderField("content-type", "application/json",
                   req->header.name_value_map);
  checkHeaderField("content-length", "46", req->header.name_value_map);
  checkBody("{\n\t\"asa-gohan\":\"misosiru\",\n\t\"oyatsu\":\"karl\"\n}\n",
            req->body);
  ASSERT_EQ(3, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::OK, req->response_status_code);
}

TEST_F(HttpRequestParserTest, StoreServerConfig) {
  std::string file_name = "simple_get.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkRequestline("GET", "/", "HTTP/1.1", req);
  checkBody("", req->body);
  ASSERT_EQ(2, req->header.name_value_map.size());
  ASSERT_EQ(4242, req->server_config.port);
  ASSERT_EQ("42tokyo", req->server_config.host);
  ASSERT_EQ("nop", req->server_config.server[0]);
  ASSERT_EQ("cluster", req->server_config.server[1]);
  ASSERT_EQ("www/html", req->server_config.root);
  ASSERT_EQ(65536, req->server_config.client_body_size_limit);
  ASSERT_EQ(HttpStatus::OK, req->response_status_code);
}

TEST_F(HttpRequestParserTest, InvalidHttpVersion) {
  std::string file_name = "invalid_http_version.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkBody("", req->body);
  ASSERT_EQ(0, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, req->response_status_code);
}

TEST_F(HttpRequestParserTest, NoRequestLine) {
  std::string file_name = "no_request_line.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkBody("", req->body);
  ASSERT_EQ(0, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, req->response_status_code);
}

TEST_F(HttpRequestParserTest, NoMethod) {
  std::string file_name = "no_method.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkBody("", req->body);
  ASSERT_EQ(0, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, req->response_status_code);
}

TEST_F(HttpRequestParserTest, NoUri) {
  std::string file_name = "no_uri.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkBody("", req->body);
  ASSERT_EQ(0, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, req->response_status_code);
}

TEST_F(HttpRequestParserTest, NoHttpVersion) {
  std::string file_name = "no_http_version.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkBody("", req->body);
  ASSERT_EQ(0, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, req->response_status_code);
}

TEST_F(HttpRequestParserTest, InvalidMethod) {
  std::string file_name = "invalid_method.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkBody("", req->body);
  ASSERT_EQ(0, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, req->response_status_code);
}

TEST_F(HttpRequestParserTest, InvalidProtocol) {
  std::string file_name = "invalid_protocol.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkBody("", req->body);
  ASSERT_EQ(0, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, req->response_status_code);
}

TEST_F(HttpRequestParserTest, VersionHasNoSlash) {
  std::string file_name = "version_has_no_slash.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkBody("", req->body);
  ASSERT_EQ(0, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, req->response_status_code);
}

TEST_F(HttpRequestParserTest, VersionHasNoPeriod) {
  std::string file_name = "version_has_no_period.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkBody("", req->body);
  ASSERT_EQ(0, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, req->response_status_code);
}

TEST_F(HttpRequestParserTest, VersionHasOtherThanDigit) {
  std::string file_name = "version_has_other_than_digit.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkBody("", req->body);
  ASSERT_EQ(0, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, req->response_status_code);
}

TEST_F(HttpRequestParserTest, VersionIsNotSupported) {
  std::string file_name = "version_is_not_supported.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkBody("", req->body);
  ASSERT_EQ(0, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::HTTP_VERSION_NOT_SUPPORTED, req->response_status_code);
}

TEST_F(HttpRequestParserTest, HeaderHasNoColon) {
  std::string file_name = "header_has_no_colon.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkBody("", req->body);
  ASSERT_EQ(0, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, req->response_status_code);
}

TEST_F(HttpRequestParserTest, HeaderHasSpaceBeforeColon) {
  std::string file_name = "header_has_space_before_colon.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkBody("", req->body);
  ASSERT_EQ(0, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, req->response_status_code);
}

TEST_F(HttpRequestParserTest, HeaderHasNoFieldName) {
  std::string file_name = "header_has_no_field_name.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkBody("", req->body);
  ASSERT_EQ(0, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, req->response_status_code);
}

TEST_F(HttpRequestParserTest, CheckFieldValueIsTrimmedByWS) {
  std::string file_name = "field_value_is_trimmed.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkRequestline("GET", "/", "HTTP/1.1", req);
  checkBody("", req->body);
  checkHeaderField("host", "admin", req->header.name_value_map);
  ASSERT_EQ(2, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::OK, req->response_status_code);
}

TEST_F(HttpRequestParserTest, FieldValueHasOnlyWS) {
  std::string file_name = "field_value_has_only_WS.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkRequestline("GET", "/", "HTTP/1.1", req);
  checkBody("", req->body);
  checkHeaderField("host", "admin", req->header.name_value_map);
  ASSERT_EQ(2, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::OK, req->response_status_code);
}

TEST_F(HttpRequestParserTest, FieldNameHasOnlyWS) {
  std::string file_name = "field_name_has_only_WS.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkBody("", req->body);
  ASSERT_EQ(0, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, req->response_status_code);
}

TEST_F(HttpRequestParserTest, FieldNameHasInvalidChar) {
  std::string file_name = "field_name_has_invalid_char.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkBody("", req->body);
  ASSERT_EQ(0, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, req->response_status_code);
}

TEST_F(HttpRequestParserTest, FieldNameLastIsInvalidChar) {
  std::string file_name = "field_name_last_is_invalid_char.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkBody("", req->body);
  ASSERT_EQ(0, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, req->response_status_code);
}

TEST_F(HttpRequestParserTest, NoHost) {
  std::string file_name = "no_host.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkBody("", req->body);
  ASSERT_EQ(0, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, req->response_status_code);
}

TEST_F(HttpRequestParserTest, NoHostEmpty) {
  std::string file_name = "no_host_empty.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkBody("", req->body);
  ASSERT_EQ(0, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, req->response_status_code);
}

TEST_F(HttpRequestParserTest, TwoHost) {
  std::string file_name = "two_host.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkBody("", req->body);
  ASSERT_EQ(0, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, req->response_status_code);
}

TEST_F(HttpRequestParserTest, SameHeaderFieldName) {
  std::string file_name = "same_header_field_name.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkRequestline("GET", "/", "HTTP/1.1", req);
  checkBody("", req->body);
  checkHeaderField("host", "admin", req->header.name_value_map);
  checkHeaderField("user-agent", "agent1,agent2", req->header.name_value_map);
  ASSERT_EQ(2, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::OK, req->response_status_code);
}

TEST_F(HttpRequestParserTest, FieldNameCaseInsesitive) {
  std::string file_name = "field_name_case_insensitive.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkRequestline("GET", "/", "HTTP/1.1", req);
  checkBody("", req->body);
  checkHeaderField("host", "admin", req->header.name_value_map);
  checkHeaderField("user-agent", "agent1", req->header.name_value_map);
  checkHeaderField("accept", "text/html", req->header.name_value_map);
  ASSERT_EQ(3, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::OK, req->response_status_code);
}

TEST_F(HttpRequestParserTest, NoCrRequestLine) {
  std::string file_name = "no_cr_request_line.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkBody("", req->body);
  ASSERT_EQ(0, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, req->response_status_code);
}

TEST_F(HttpRequestParserTest, NoCrHeaderField) {
  std::string file_name = "no_cr_header_field.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkBody("", req->body);
  ASSERT_EQ(0, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, req->response_status_code);
}

TEST_F(HttpRequestParserTest, NoCrHeaderEnd) {
  std::string file_name = "no_cr_header_end.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkBody("", req->body);
  ASSERT_EQ(0, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::BAD_REQUEST, req->response_status_code);
}

TEST_F(HttpRequestParserTest, NoCrBodyEnd) {
  std::string file_name = "no_cr_body_end.crlf";
  HttpRequest *req = buildRequest(file_dir + file_name, config);

  checkBody("", req->body);
  ASSERT_EQ(2, req->header.name_value_map.size());
  ASSERT_EQ(HttpStatus::OK, req->response_status_code);
}
