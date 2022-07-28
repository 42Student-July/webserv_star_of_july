#ifndef SRC_REQUESTHEADERPARSER_HPP_
#define SRC_REQUESTHEADERPARSER_HPP_

#include <iostream>
#include <map>
#include <string>

#include "HeaderFieldParser.hpp"
#include "HttpParser.hpp"
#include "HttpRequest.hpp"
#include "HttpStatus.hpp"
#include "RequestLineParser.hpp"
#include "ServerConfig.hpp"

class RequestHeaderParser : public HttpParser {
 public:
  RequestHeaderParser();
  ~RequestHeaderParser();
  // 2つの引数はコンストラクタで渡した方が読みやすいかも。
  RequestHeader parse(const std::string &unparsed_str);

 private:
  static const size_t kMaxHeaderLength = 1 << 10;
  // static const size_t kMaxBodyLength = 1 << 20;

  RequestHeaderParser(const RequestHeaderParser &other);
  RequestHeaderParser &operator=(const RequestHeaderParser &other);
  static void validateRequestLength(const std::string &unparsed_str);
  static RequestLine parseRequestLine(const std::string &unparsed_str,
                                      StringPos *offset);
  HeaderFieldMap parseHeaderField(const std::string &unparsed_str,
                                  StringPos *offset);
  // static std::string getLine(const std::string &unparsed_str,
  //                            StringPos *offset);
};

#endif  // SRC_REQUESTHEADERPARSER_HPP_
