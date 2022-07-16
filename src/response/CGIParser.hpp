#ifndef CGIPARSER_HPP
#define CGIPARSER_HPP

#include <iostream>

#include "HttpResponseBuilder.hpp"

class CGIParser {
 public:
  //全体でのtypedefにしたい
  typedef std::map<std::string, std::string> HeaderFieldMap;

  CGIParser();
  virtual ~CGIParser();

  void parse(std::string cgi_response);
  void parseHeader();
  void parseBody();
  bool getLine(std::string *line);

 private:
  static const std::string CRLF;
  static const std::string WS;
  CGIParser(CGIParser const &other);
  CGIParser &operator=(CGIParser const &other);

  std::string offset_;
  std::string cgi_response_;

  // std::map<std::string, std::string> header_;
  std::stringstream file_str_;
};

#endif
