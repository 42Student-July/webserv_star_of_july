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
  static std::string parseMethod(const std::string& line, StringPos* offset);
  static std::string parseUri(const std::string& line, StringPos* offset);
  static std::string parseHttpVersion(const std::string& line,
                                      StringPos offset);
  static void validateRequestLine(const RequestLine& request_line);
  static void validateMethod(const std::string& method);
  static void validateUri(const std::string& uri);
  static void validateHttpVersion(const std::string& http_version);
};

#endif /* REQUESTLINEPARSER_HPP */
