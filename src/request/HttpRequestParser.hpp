#ifndef SRC_HTTPREQUESTPARSER_HPP_
#define SRC_HTTPREQUESTPARSER_HPP_

#include <iostream>
#include <map>
#include <string>

#include "HeaderFieldParser.hpp"
#include "HttpParser.hpp"
#include "HttpRequest.hpp"
#include "HttpStatus.hpp"
#include "RequestLineParser.hpp"
#include "ServerConfig.hpp"

class HttpRequestParser : public HttpParser {
 public:
  HttpRequestParser();
  ~HttpRequestParser();
  // 2つの引数はコンストラクタで渡した方が読みやすいかも。
  HttpRequest *parse(const char *buffer, const ServerConfig &server_config);

 private:
  HttpRequestParser(const HttpRequestParser &other);
  HttpRequestParser &operator=(const HttpRequestParser &other);
  RequestLine parseRequestLine(const std::string &buffer, StringPos *offset);
  HeaderFieldMap parseHeaderField(const std::string &buffer, StringPos *offset);
  std::string parseBody(const std::string &buffer, StringPos offset);
  std::string getLine(const std::string &buffer, StringPos *offset);
};

#endif  // SRC_HTTPREQUESTPARSER_HPP_
