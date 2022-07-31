#include <gtest/gtest.h>

#include "HttpRequest.hpp"
#include "HttpRequestConverter.hpp"
#include "HttpRequestDTO.hpp"
#include "HttpRequestParser.hpp"
#include "ServerConfig.hpp"
#include "test_helper.hpp"

const size_t kPort = 4242;

class HttpConverterTest : public ::testing::Test {
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
    conf_vec.push_back(config);
    wconfig = WebservConfig(conf_vec);
  }
  ServerConfig config;
  ServConfVector conf_vec;
  WebservConfig wconfig;
};

TEST_F(HttpConverterTest, StoreOneHeaderField) {
  std::string file_name = "dto_one_header_field.crlf";
  HttpRequestDTO *dto = buildDTO(file_dir + file_name, kPort, wconfig);

  checkRequestline("POST", "/", "1.1", dto);
  checkHeaderField("Basic fugafuga==", dto->authorization);
  ASSERT_EQ(HttpStatus::OK, dto->response_status_code);
}

TEST_F(HttpConverterTest, StoreNoHeaderField) {
  std::string file_name = "dto_no_header_field.crlf";
  HttpRequestDTO *dto = buildDTO(file_dir + file_name, kPort, wconfig);

  checkRequestline("POST", "/", "1.1", dto);
  checkHeaderField("", dto->authorization);
  checkHeaderField("0", dto->content_length);
  checkHeaderField("", dto->content_type);
  checkHeaderField("", dto->accept);
  checkHeaderField("", dto->forwarded);
  checkHeaderField("", dto->referer);
  checkHeaderField("", dto->user_agent);
  checkHeaderField("", dto->x_forwarded_for);
  ASSERT_EQ(HttpStatus::OK, dto->response_status_code);
}

TEST_F(HttpConverterTest, StoreAllHeaderField) {
  std::string file_name = "dto_all_header_field.crlf";
  HttpRequestDTO *dto = buildDTO(file_dir + file_name, kPort, wconfig);

  checkRequestline("POST", "/", "1.1", dto);
  checkHeaderField("keep-alive", dto->connection);
  checkHeaderField("Basic fugafuga==", dto->authorization);
  checkHeaderField("21", dto->content_length);
  checkHeaderField("text/html", dto->content_type);
  checkHeaderField("text/html,application/json", dto->accept);
  checkHeaderField("?", dto->forwarded);
  checkHeaderField("??", dto->referer);
  checkHeaderField("gtest", dto->user_agent);
  checkHeaderField("???", dto->x_forwarded_for);
  ASSERT_EQ(HttpStatus::OK, dto->response_status_code);
}

TEST_F(HttpConverterTest, StoreAllHeaderFieldShuffled) {
  std::string file_name = "dto_all_header_field_shuffled.crlf";
  HttpRequestDTO *dto = buildDTO(file_dir + file_name, kPort, wconfig);

  checkRequestline("POST", "/", "1.1", dto);
  checkHeaderField("keep-alive", dto->connection);
  checkHeaderField("Basic fugafuga==", dto->authorization);
  checkHeaderField("21", dto->content_length);
  checkHeaderField("text/html", dto->content_type);
  checkHeaderField("text/html,application/json", dto->accept);
  checkHeaderField("?", dto->forwarded);
  checkHeaderField("??", dto->referer);
  checkHeaderField("gtest", dto->user_agent);
  checkHeaderField("???", dto->x_forwarded_for);
  ASSERT_EQ(HttpStatus::OK, dto->response_status_code);
}

TEST_F(HttpConverterTest, StoreMultiLinesToBody) {
  std::string file_name = "body_multi_lines.crlf";
  HttpRequestDTO *dto = buildDTO(file_dir + file_name, kPort, wconfig);

  checkRequestline("POST", "/", "1.1", dto);
  checkBody("1stline\n2ndline\n3rdline\n", dto->body);
  ASSERT_EQ(HttpStatus::OK, dto->response_status_code);
}

TEST_F(HttpConverterTest, StoreServerConfig) {
  std::string file_name = "simple_get.crlf";
  HttpRequestDTO *dto = buildDTO(file_dir + file_name, kPort, wconfig);

  checkRequestline("GET", "/", "1.1", dto);
  checkBody("", dto->body);
  ASSERT_EQ(HttpStatus::OK, dto->response_status_code);
  ASSERT_EQ(4242, dto->port);
  ASSERT_EQ("42tokyo", dto->host);
  ASSERT_EQ("nop", dto->servernames[0]);
  ASSERT_EQ("cluster", dto->servernames[1]);
}

TEST_F(HttpConverterTest, NoHttpVersion) {
  std::string file_name = "no_http_version.crlf";
  HttpRequestDTO *dto = buildDTO(file_dir + file_name, kPort, wconfig);

  checkRequestline("", "", "", dto);
  checkBody("", dto->body);
  ASSERT_EQ(HttpStatus::BAD_REQUEST, dto->response_status_code);
}
