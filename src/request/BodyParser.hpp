#ifndef BODYPARSER_HPP
#define BODYPARSER_HPP

#include <iostream>  //デバッグ用

#include "HttpParser.hpp"

class BodyParser : public HttpParser {
 public:
  BodyParser();
  ~BodyParser();
  std::string parse(const std::string& buffer, bool is_chunked,
                    size_t content_length);

 private:
  BodyParser(const BodyParser& other);
  BodyParser& operator=(const BodyParser& other);
  std::string parseBody(const std::string& buffer, size_t content_length);
};

#endif /* BODYPARSER_HPP */
