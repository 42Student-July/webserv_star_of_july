#ifndef SRC_HTTPREQUESTPARSER_HPP_
#define SRC_HTTPREQUESTPARSER_HPP_

#include <iostream>
#include <map>
#include <string>

#include "HttpRequest.hpp"
#include "ServerConfig.hpp"

class HttpRequestParser {
 public:
  // privateにするかも
  typedef std::map<std::string, std::string> HeaderFieldMap;

  HttpRequestParser();
  ~HttpRequestParser();

  HttpRequest* parse(const char* request_str,
                     const ServerConfig& server_config);

 private:
  // 型
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
  void validateRequestLine(HttpRequest* request);
  void parseHeaderField(HttpRequest* request);
  void parseBody(HttpRequest* request);
  StringPos parseMethod(const std::string& request_line, HttpRequest* request);
  StringPos parseUri(const std::string& request_line, HttpRequest* request,
                     StringPos offset);
  StringPos parseHttpVersion(const std::string& request_line,
                             HttpRequest* request, StringPos offset);
  bool getLine(std::string* line);

  // メンバ変数
  std::string offset_;
};

#endif  // SRC_HTTPREQUESTPARSER_HPP_
