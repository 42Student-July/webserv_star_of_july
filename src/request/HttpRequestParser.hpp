#ifndef SRC_HTTPREQUESTPARSER_HPP_
#define SRC_HTTPREQUESTPARSER_HPP_

#include <iostream>
#include <map>
#include <string>

#include "HttpParser.hpp"
#include "HttpRequest.hpp"
#include "HttpStatus.hpp"
#include "RequestLineParser.hpp"
#include "ServerConfig.hpp"

class HttpRequestParser : public HttpParser {
 public:
  HttpRequestParser();
  ~HttpRequestParser();
  // 2つの引数はコンストラクタで渡した方が読みやすいかも。
  HttpRequest* parse(const char* request_str,
                     const ServerConfig& server_config);

 private:
  // メソッド
  HttpRequestParser(const HttpRequestParser& other);
  HttpRequestParser& operator=(const HttpRequestParser& other);
  void parseRequestLine(HttpRequest* request);
  void parseHeaderField(HttpRequest* request);
  void parseBody(HttpRequest* request);
  static HeaderFieldPair makeHeaderFieldPair(const std::string& line);
  static void validateHeaderField(const HeaderFieldPair& headerfield_pair);
  static void validateHeaderFields(const HeaderFieldMap& headers);
  static bool isHeaderDelimiter(int c);
  static bool isHeaderTokenChar(int c);
  static bool isHeaderToken(const std::string& str);
  std::string getLine();

  // メンバ変数
  std::string raw_buffer_;
  StringPos offset_;
};

#endif  // SRC_HTTPREQUESTPARSER_HPP_
