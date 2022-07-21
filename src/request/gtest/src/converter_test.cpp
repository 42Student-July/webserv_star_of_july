#include <gtest/gtest.h>

#include "HttpRequest.hpp"
#include "HttpRequestConverter.hpp"
#include "HttpRequestDTO.hpp"
#include "HttpRequestParser.hpp"
#include "ServerConfig.hpp"
#include "test_helper.hpp"

TEST(HttpConverterTest, StoreOneHeaderField) {
  HttpRequestParser parser;
  ServerConfig config = initServerCongig();
  std::string file_content = readFile("request/dto_one_header_field.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  HttpRequestConverter converter;
  HttpRequestDTO *dto = converter.toDTO(request);

  checkRequestline("POST", "/", "1.1", dto);
  checkHeaderField("Basic fugafuga==", dto->authorization);
  compareString(HttpStatus::OK, dto->response_status_code);
}

TEST(HttpConverterTest, StoreNoHeaderField) {
  HttpRequestParser parser;
  ServerConfig config = initServerCongig();
  std::string file_content = readFile("request/dto_no_header_field.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  HttpRequestConverter converter;
  HttpRequestDTO *dto = converter.toDTO(request);

  checkRequestline("POST", "/", "1.1", dto);
  checkHeaderField("", dto->authorization);
  checkHeaderField("", dto->content_length);
  checkHeaderField("", dto->content_type);
  checkHeaderField("", dto->accept);
  checkHeaderField("", dto->forwarded);
  checkHeaderField("", dto->referer);
  checkHeaderField("", dto->user_agent);
  checkHeaderField("", dto->x_forwarded_for);
  compareString(HttpStatus::OK, dto->response_status_code);
}

TEST(HttpConverterTest, StoreAllHeaderField) {
  HttpRequestParser parser;
  ServerConfig config = initServerCongig();
  std::string file_content = readFile("request/dto_all_header_field.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  HttpRequestConverter converter;
  HttpRequestDTO *dto = converter.toDTO(request);

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
  compareString(HttpStatus::OK, dto->response_status_code);
}

TEST(HttpConverterTest, StoreAllHeaderFieldShuffled) {
  HttpRequestParser parser;
  ServerConfig config = initServerCongig();
  std::string file_content =
      readFile("request/dto_all_header_field_shuffled.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  HttpRequestConverter converter;
  HttpRequestDTO *dto = converter.toDTO(request);

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
  compareString(HttpStatus::OK, dto->response_status_code);
}

TEST(HttpConverterTest, StoreBodyMultiLinesToBody) {
  HttpRequestParser parser;
  ServerConfig config = initServerCongig();
  std::string file_content = readFile("request/body_multi_lines.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  HttpRequestConverter converter;
  HttpRequestDTO *dto = converter.toDTO(request);

  checkRequestline("POST", "/", "1.1", dto);
  checkBody("1stline\n2ndline\n3rdline\n", dto->body);
  compareString(HttpStatus::OK, dto->response_status_code);
}

TEST(HttpConverterTest, StoreServerConfig) {
  HttpRequestParser parser;
  ServerConfig config = initServerCongig();
  std::string file_content = readFile("request/simple_get.crlf");
  HttpRequest *request = parser.parse(file_content.c_str(), config);

  HttpRequestConverter converter;
  HttpRequestDTO *dto = converter.toDTO(request);

  checkRequestline("GET", "/", "1.1", dto);
  checkBody("", dto->body);
  compareString(HttpStatus::OK, dto->response_status_code);
  ASSERT_EQ(4242, dto->port);
  compareString("42tokyo", dto->host);
  compareString("nop", dto->servernames[0]);
  compareString("cluster", dto->servernames[1]);
}
