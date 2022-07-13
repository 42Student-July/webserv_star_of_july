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

  HttpRequestDTO* parse(const char* request_str);
  const HeaderFieldMap& getHeaderFieldMap() const;

 private:
  static const std::string CRLF;
  static const std::string WS;

  HttpRequestParser(const HttpRequestParser& other);
  HttpRequestParser& operator=(const HttpRequestParser& other);

  void parseRequestLine(HttpRequestDTO* request);
  void parseHeaderField(HttpRequestDTO* request);
  // void parseBody();
  void storeHeaderField();
  void setHeaderField(HttpRequestDTO* request);
  bool getLine(std::string* line);

  std::string offset_;
  HeaderFieldMap name_value_map_;
};

#endif  // SRC_HTTPREQUESTPARSER_HPP_
