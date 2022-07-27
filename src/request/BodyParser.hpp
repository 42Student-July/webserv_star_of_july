#ifndef BODYPARSER_HPP
#define BODYPARSER_HPP

#include <iostream>  //デバッグ用

#include "HttpParser.hpp"
#include "utility.hpp"

class BodyParser : public HttpParser {
 public:
  BodyParser();
  ~BodyParser();
  std::string parse(const std::string& buffer, bool is_chunked,
                    bool exists_content_length, size_t content_length);

 private:
  BodyParser(const BodyParser& other);
  BodyParser& operator=(const BodyParser& other);
  std::string parseBody(const std::string& buffer, bool exists_content_length,
                        size_t content_length);
  std::string parseChunkedBody(const std::string& buffer);
  std::string getLine(const std::string& buffer, StringPos* offset);
};

#endif /* BODYPARSER_HPP */
