#ifndef SRC_HTTPPARSER_HPP_
#define SRC_HTTPPARSER_HPP_

#include "HttpRequest.hpp"

class HttpParser {
 public:
  HttpParser();
  ~HttpParser();

 protected:
  typedef HttpRequest::HeaderFieldPair HeaderFieldPair;
  typedef HttpRequest::HeaderFieldMap HeaderFieldMap;
  typedef std::string::size_type StringPos;
  typedef std::vector<std::string> StringVector;

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

 private:
  HttpParser(const HttpParser& other);
  HttpParser& operator=(const HttpParser& other);
};

#endif  // SRC_HTTPPARSER_HPP_
