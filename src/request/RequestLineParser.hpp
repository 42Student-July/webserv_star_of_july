#ifndef REQUESTLINEPARSER_HPP
#define REQUESTLINEPARSER_HPP

#include "HttpParser.hpp"
#include "RequestLine.hpp"

class RequestLineParser : public HttpParser {
 public:
  RequestLineParser();
  ~RequestLineParser();
  RequestLine parse(const std::string& line);

 private:
  RequestLineParser(const RequestLineParser& other);
  RequestLineParser& operator=(const RequestLineParser& other);
  std::string parseMethod(const std::string& line, StringPos* offset);
  std::string parseUri(const std::string& line, StringPos* offset);
  std::string parseHttpVersion(const std::string& line, StringPos offset);
  void validateRequestLine(const RequestLine& request_line);
};

#endif /* REQUESTLINEPARSER_HPP */
