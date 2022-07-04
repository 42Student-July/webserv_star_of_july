#ifndef HTTPSERVER_SRC_HTTPREQUESTPARSER_HPP_
#define HTTPSERVER_SRC_HTTPREQUESTPARSER_HPP_

#include <iostream>
#include <string>

#include "HttpRequest.hpp"

class HttpRequestParser {
 public:
  HttpRequestParser();
  ~HttpRequestParser();

  HttpRequest* parse(const char* buffer);

 private:
  HttpRequestParser(const HttpRequestParser& other);
  HttpRequestParser& operator=(const HttpRequest& other);

  void parseRequestLine();
  void parseHeaderField();
  bool getLine(std::string& line);
  void setBody(const char* buffer);

  HttpRequest* current_request_;
  std::string current_buffer_;
  std::string buffer_offset_;
};

#endif  // HTTPSERVER_SRC_HTTPREQUESTPARSER_HPP_
