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
  static const size_t kMaxHeaderLength = 1 << 10;
  static const size_t kMaxBodyLength = 1 << 20;

  HttpRequestParser(const HttpRequestParser &other);
  HttpRequestParser &operator=(const HttpRequestParser &other);
  static void validateRequestLength(const std::string &unparsed_str);
  static RequestHeader parseRequestHeader(const std::string &unparsed_str);
  std::string parseBody(const std::string &unparsed_str);
  std::string parseMessageBody(const std::string &unparsed_str,
                               const HttpRequest *request);
  static std::string getLine(const std::string &unparsed_str,
                             StringPos *offset);
  void setContentLengthInfo(HeaderFieldMap &headerfield_map, HttpRequest *req);
};

#endif  // SRC_HTTPREQUESTPARSER_HPP_
