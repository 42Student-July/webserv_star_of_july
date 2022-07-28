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
  enum Status {
    PARSE_HEADER,
    PARSE_BODY,
    PARSE_CHUNKED_BODY,
    PARSE_DONE,
    PARSE_ERROR
  };

  HttpRequestParser(const HttpRequestParser &other);
  HttpRequestParser &operator=(const HttpRequestParser &other);
  RequestHeader parseRequestHeader(const std::string &unparsed_str);
  std::string parseBody(size_t content_length);
  std::string parseChunkedBody(const std::string &unparsed_str);
  void changeStatus(Status next_status);

  Status parse_status_;
  std::string unparsed_body_;
};

#endif  // SRC_HTTPREQUESTPARSER_HPP_
