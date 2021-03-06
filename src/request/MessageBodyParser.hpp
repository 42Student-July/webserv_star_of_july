#ifndef MESSAGEBODYPARSER_HPP
#define MESSAGEBODYPARSER_HPP

#include <iostream>  //デバッグ用

#include "HttpParser.hpp"
#include "utility.hpp"

class MessageBodyParser : public HttpParser {
 public:
  MessageBodyParser();
  ~MessageBodyParser();
  std::string parseBody(const std::string& buffer, size_t content_length);
  std::string parseChunkedBody(const std::string& buffer);

 private:
  MessageBodyParser(const MessageBodyParser& other);
  MessageBodyParser& operator=(const MessageBodyParser& other);
};

#endif /* MESSAGEBODYPARSER_HPP */
