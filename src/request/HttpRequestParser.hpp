#ifndef SRC_HTTPREQUESTPARSER_HPP_
#define SRC_HTTPREQUESTPARSER_HPP_

#include <iostream>
#include <map>
#include <string>

#include "HttpRequest.hpp"
#include "HttpStatus.hpp"
#include "ServerConfig.hpp"

class HttpRequestParser {
 public:
  HttpRequestParser();
  ~HttpRequestParser();
  // 2つの引数はコンストラクタで渡した方が読みやすいかも。
  HttpRequest* parse(const char* request_str,
                     const ServerConfig& server_config);

 private:
  // 型
  typedef HttpRequest::HeaderFieldPair HeaderFieldPair;
  typedef std::string::size_type StringPos;

  class ParseErrorExeption : public std::runtime_error {
   public:
    ParseErrorExeption(const std::string& error_status,
                       const std::string& reason = "Undefined Error");
    ~ParseErrorExeption() throw();
    const std::string& getErrorStatus() const;

   private:
    std::string error_status_;
  };

  // 定数
  static const std::string CRLF;
  static const std::string WS;

  // メソッド
  HttpRequestParser(const HttpRequestParser& other);
  HttpRequestParser& operator=(const HttpRequestParser& other);
  void parseRequestLine(HttpRequest* request);
  void parseHeaderField(HttpRequest* request);
  void parseBody(HttpRequest* request);
  StringPos parseMethod(const std::string& request_line, HttpRequest* request);
  StringPos parseUri(const std::string& request_line, HttpRequest* request,
                     StringPos offset);
  StringPos parseHttpVersion(const std::string& request_line,
                             HttpRequest* request, StringPos offset);
  void validateRequestLine(HttpRequest* request);
  static HeaderFieldPair makeHeaderFieldPair(const std::string& line);
  std::string getLine();

  // メンバ変数
  std::string raw_buffer_;
  StringPos offset_;
};

#endif  // SRC_HTTPREQUESTPARSER_HPP_
