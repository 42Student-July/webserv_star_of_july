#ifndef SRC_HTTPPARSER_HPP_
#define SRC_HTTPPARSER_HPP_

#include "HttpRequest.hpp"
#include "utility.hpp"

class HttpParser {
 public:
  HttpParser();
  ~HttpParser();

  class ParseErrorExeption : public std::runtime_error {
   public:
    ParseErrorExeption(const std::string& error_status,
                       const std::string& reason = "Undefined Error");
    ~ParseErrorExeption() throw();
    const std::string& getErrorStatus() const;

   private:
    std::string error_status_;
  };

 protected:
  // 型
  typedef std::string::size_type StringPos;
  typedef std::vector<std::string> StringVector;
  // 定数
  static const std::string CRLF;
  static const std::string OWS;
  static const std::string Delimiters;
  // メソッド
  static bool isOWS(int c);
  static bool isHeaderDelimiter(int c);
  static bool isHeaderTokenChar(int c);
  static bool isHeaderToken(const std::string& str);

 private:
  HttpParser(const HttpParser& other);
  HttpParser& operator=(const HttpParser& other);
};

#endif  // SRC_HTTPPARSER_HPP_
