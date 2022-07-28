#ifndef SRC_HTTPREQUESTPARSER_HPP_
#define SRC_HTTPREQUESTPARSER_HPP_

#include <iostream>
#include <map>
#include <string>

#include "HttpParser.hpp"
#include "HttpRequest.hpp"
#include "HttpStatus.hpp"
#include "MessageBodyParser.hpp"
#include "RequestHeaderParser.hpp"
#include "ServerConfig.hpp"

class HttpRequestParser : public HttpParser {
 public:
  HttpRequestParser();
  ~HttpRequestParser();
  // 2つの引数はコンストラクタで渡した方が読みやすいかも。
  HttpRequest *parse(const std::string unparsed_str,
                     const ServerConfig &server_config);

 private:
  enum Status { PARSE_HEADER, PARSE_BODY, PARSE_DONE, PARSE_ERROR };

  static const size_t kMaxHeaderLength = 1 << 10;
  static const size_t kMaxBodyLength = 1 << 20;

  HttpRequestParser(const HttpRequestParser &other);
  HttpRequestParser &operator=(const HttpRequestParser &other);
  static void validateRequestLength(const std::string &unparsed_str);
  static RequestHeader parseRequestHeader(const std::string &unparsed_str);
  std::string parseBody(const std::string &unparsed_str);
  std::string parseMessageBody(const std::string &unparsed_str,
                               const RequestHeader &header);
  // void changeStatus(Status next_status);
  // Status parse_status_;
};

#endif  // SRC_HTTPREQUESTPARSER_HPP_
