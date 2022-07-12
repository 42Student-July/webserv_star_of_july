#ifndef SRC_HTTPREQUESTPARSER_HPP_
#define SRC_HTTPREQUESTPARSER_HPP_

#include <iostream>
#include <string>

#include "HttpRequestDTO.hpp"

class HttpRequestParser {
 public:
  HttpRequestParser();
  ~HttpRequestParser();

  HttpRequestDTO* parse(const char* buffer);

 private:
  static const std::string CRLF;
  HttpRequestParser(const HttpRequestParser& other);
  HttpRequestParser& operator=(const HttpRequestParser& other);

  void parseRequestLine();
  void parseHeaderField();
  bool getLine(std::string* line);
  void setBody(const char* buffer);

  HttpRequestDTO* current_request_;
  std::string current_buffer_;
  std::string buffer_offset_;
};

#endif  // SRC_HTTPREQUESTPARSER_HPP_
