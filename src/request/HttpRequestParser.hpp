#ifndef SRC_HTTPREQUESTPARSER_HPP_
#define SRC_HTTPREQUESTPARSER_HPP_

#include <iostream>
#include <map>
#include <string>

#include "HttpRequest.hpp"
#include "ServerConfig.hpp"

class HttpRequestParser {
 public:
  // privateにするかも
  typedef std::map<std::string, std::string> HeaderFieldMap;

  HttpRequestParser();
  ~HttpRequestParser();

  HttpRequest* parse(const char* request_str,
                     const ServerConfig& server_config);

 private:
  static const std::string CRLF;
  static const std::string WS;

  HttpRequestParser(const HttpRequestParser& other);
  HttpRequestParser& operator=(const HttpRequestParser& other);

  void parseRequestLine(HttpRequest* request);
  void parseHeaderField(HttpRequest* request);
  void parseBody(HttpRequest* request);
  bool getLine(std::string* line);

  std::string offset_;
};

#endif  // SRC_HTTPREQUESTPARSER_HPP_
