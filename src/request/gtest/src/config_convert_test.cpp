#include <gtest/gtest.h>

// #include "../../../response/ConfigDTO.hpp"
#include "ConfigConverter.hpp"
#include "ConfigDTO.hpp"
#include "HttpRequest.hpp"
#include "HttpRequestConverter.hpp"
#include "HttpRequestDTO.hpp"
#include "HttpRequestParser.hpp"
#include "ServerConfig.hpp"
#include "test_helper.hpp"

TEST(ConfigConverterTest, StoreEverything) {
  ServerConfig config = initServerConfigWithLocation();
  ConfigConverter converter;
  ConfigDTO *dto = converter.toDTO(config);

  EXPECT_EQ("4242", dto->port);
  EXPECT_EQ("42tokyo", dto->host);
  EXPECT_EQ("nop", dto->server);
  EXPECT_EQ("error0", dto->error_pages[0]);
  EXPECT_EQ("www/html", dto->root);
  EXPECT_EQ("location", dto->locations[0].location);
  EXPECT_EQ("www/cgi-bin", dto->locations[0].root);
  EXPECT_EQ("GET", dto->locations[0].allowed_methods[0]);
  EXPECT_EQ("index.cgi", dto->locations[0].indexes[0]);
  EXPECT_EQ(false, dto->locations[0].autoindex);
  EXPECT_EQ(".py", dto->locations[0].cgi_extensions[0]);
}

/* TEST(HttpConverterTest, StoreNoHeaderField) { */
/*   HttpRequestParser parser; */
/*   ServerConfig config = initServerCongig(); */
/*   std::string file_content = readFile("request/dto_no_header_field.crlf"); */
/*   HttpRequest *request = parser.parse(file_content.c_str(), config); */

/*   HttpRequestConverter converter; */
/*   HttpRequestDTO *dto = converter.toDTO(request); */

/*   checkRequestline("POST", "/", "HTTP/1.1", dto); */
/*   checkHeaderField("", dto->authorization); */
/*   checkHeaderField("", dto->content_length); */
/*   checkHeaderField("", dto->content_type); */
/*   checkHeaderField("", dto->accept); */
/*   checkHeaderField("", dto->forwarded); */
/*   checkHeaderField("", dto->referer); */
/*   checkHeaderField("", dto->user_agent); */
/*   checkHeaderField("", dto->x_forwarded_for); */
/*   ASSERT_FALSE(dto->is_bad_request); */
/* } */

/* TEST(HttpConverterTest, StoreAllHeaderField) { */
/*   HttpRequestParser parser; */
/*   ServerConfig config = initServerCongig(); */
/*   std::string file_content = readFile("request/dto_all_header_field.crlf");
 */
/*   HttpRequest *request = parser.parse(file_content.c_str(), config); */

/*   HttpRequestConverter converter; */
/*   HttpRequestDTO *dto = converter.toDTO(request); */

/*   checkRequestline("POST", "/", "HTTP/1.1", dto); */
/*   checkHeaderField("Basic fugafuga==", dto->authorization); */
/*   checkHeaderField("21", dto->content_length); */
/*   checkHeaderField("text/html", dto->content_type); */
/*   checkHeaderField("text/html,application/json", dto->accept); */
/*   checkHeaderField("?", dto->forwarded); */
/*   checkHeaderField("??", dto->referer); */
/*   checkHeaderField("gtest", dto->user_agent); */
/*   checkHeaderField("???", dto->x_forwarded_for); */
/*   ASSERT_FALSE(dto->is_bad_request); */
/* } */

/* TEST(HttpConverterTest, StoreAllHeaderFieldShuffled) { */
/*   HttpRequestParser parser; */
/*   ServerConfig config = initServerCongig(); */
/*   std::string file_content = */
/*       readFile("request/dto_all_header_field_shuffled.crlf"); */
/*   HttpRequest *request = parser.parse(file_content.c_str(), config); */

/*   HttpRequestConverter converter; */
/*   HttpRequestDTO *dto = converter.toDTO(request); */

/*   checkRequestline("POST", "/", "HTTP/1.1", dto); */
/*   checkHeaderField("Basic fugafuga==", dto->authorization); */
/*   checkHeaderField("21", dto->content_length); */
/*   checkHeaderField("text/html", dto->content_type); */
/*   checkHeaderField("text/html,application/json", dto->accept); */
/*   checkHeaderField("?", dto->forwarded); */
/*   checkHeaderField("??", dto->referer); */
/*   checkHeaderField("gtest", dto->user_agent); */
/*   checkHeaderField("???", dto->x_forwarded_for); */
/*   ASSERT_FALSE(dto->is_bad_request); */
/* } */

/* TEST(HttpConverterTest, StoreBodyMultiLinesToBody) { */
/*   HttpRequestParser parser; */
/*   ServerConfig config = initServerCongig(); */
/*   std::string file_content = readFile("request/body_multi_lines.crlf"); */
/*   HttpRequest *request = parser.parse(file_content.c_str(), config); */

/*   HttpRequestConverter converter; */
/*   HttpRequestDTO *dto = converter.toDTO(request); */

/*   checkRequestline("POST", "/", "HTTP/1.1", dto); */
/*   checkBody("1stline\n2ndline\n3rdline\n", dto->body); */
/*   ASSERT_FALSE(dto->is_bad_request); */
/* } */

/* TEST(HttpConverterTest, StoreServerConfig) { */
/*   HttpRequestParser parser; */
/*   ServerConfig config = initServerCongig(); */
/*   std::string file_content = readFile("request/simple_get.crlf"); */
/*   HttpRequest *request = parser.parse(file_content.c_str(), config); */

/*   HttpRequestConverter converter; */
/*   HttpRequestDTO *dto = converter.toDTO(request); */

/*   checkRequestline("GET", "/", "HTTP/1.1", dto); */
/*   checkBody("", dto->body); */
/*   ASSERT_FALSE(dto->is_bad_request); */
/*   ASSERT_EQ(4242, dto->port); */
/*   compareString("42tokyo", dto->host); */
/*   compareString("nop", dto->servernames[0]); */
/*   compareString("cluster", dto->servernames[1]); */
/* } */
