#ifndef SRC_HTTPREQUESTPARSER_HPP_
#define SRC_HTTPREQUESTPARSER_HPP_

#include <iostream>
#include <map>
#include <string>

#include "HttpRequestDTO.hpp"

class HttpRequestParser {
 public:
  // privateにするかも
  typedef std::map<std::string, std::string> HeaderFieldMap;

  HttpRequestParser();
  ~HttpRequestParser();

  HttpRequestDTO* parse(const char* buffer);
  const HeaderFieldMap& getHeaderFieldMap() const;

 private:
  static const std::string CRLF;
  static const std::string WS;

  HttpRequestParser(const HttpRequestParser& other);
  HttpRequestParser& operator=(const HttpRequestParser& other);

  void parseRequestLine();
  void parseHeaderField();
  bool getLine(std::string* line);
  void setBody(const char* buffer);

  HttpRequestDTO* current_request_;
  std::string current_buffer_;
  std::string buffer_offset_;
  HeaderFieldMap name_value_map_;
};

#endif  // SRC_HTTPREQUESTPARSER_HPP_
