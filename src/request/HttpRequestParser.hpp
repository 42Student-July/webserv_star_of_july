#ifndef SRC_HTTPREQUESTPARSER_HPP_
#define SRC_HTTPREQUESTPARSER_HPP_

#include <iostream>
#include <map>
#include <string>

#include "HttpParser.hpp"
#include "HttpRequest.hpp"
#include "HttpStatus.hpp"
#include "RequestHeaderParser.hpp"
#include "ServerConfig.hpp"

class HttpRequestParser : public HttpParser {
 public:
  HttpRequestParser();
  ~HttpRequestParser();
  // 2つの引数はコンストラクタで渡した方が読みやすいかも。
  HttpRequest *parse(const std::string buffer,
                     const ServerConfig &server_config);
  // ここで持つべきじゃないので後で移動
  const std::string &getBodyBuffer() const;

 private:
  static const size_t kMaxHeaderLength = 1 << 10;
  static const size_t kMaxBodyLength = 1 << 20;

  HttpRequestParser(const HttpRequestParser &other);
  HttpRequestParser &operator=(const HttpRequestParser &other);
  static void validateRequestLength(const std::string &buffer);
  static RequestHeader parseRequestHeader(const std::string &buffer);
  static std::string parseBody(const std::string &buffer);
  static std::string getLine(const std::string &buffer, StringPos *offset);
  void setContentLengthInfo(HeaderFieldMap &headerfield_map, HttpRequest *req);

  // ここで持つべきじゃないので後で移動
  std::string body_buffer_;
};

#endif  // SRC_HTTPREQUESTPARSER_HPP_
