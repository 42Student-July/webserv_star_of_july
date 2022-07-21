#ifndef CGIPARSER_HPP
#define CGIPARSER_HPP

#include <iostream>
#include <string>
#include <sstream>

#include "HttpResponseBuilder.hpp"
#include "HttpResponseHeaderDTO.hpp"

class CGIParser {
 public:
  //全体でのtypedefにしたい
  typedef std::map<std::string, std::string> HeaderFieldMap;

  CGIParser();
  virtual ~CGIParser();

  void parse(std::string cgi_response);
  void parseHeaderField(std::stringstream &ss);
  void parseBody(std::stringstream &ss);

  std::string getBodyStr() const;
  HttpResponseHeaderDTO getHeader() const;
  std::map<std::string, std::string> getCGIHeader() const;
  std::pair<std::string, std::string> makeHeaderFieldPair(
      const std::string &line);

  std::string getLine();

 private:
  typedef std::string::size_type StringPos;
  static const std::string CRLF;
  static const std::string WS;

  CGIParser(CGIParser const &other);
  CGIParser &operator=(CGIParser const &other);

  StringPos offset_;
  std::string raw_buffer_;
  std::string cgi_response_;

  std::map<std::string, std::string> cgi_header_;
  std::string res_body_str_;
  HttpResponseHeaderDTO header_;
};

#endif
