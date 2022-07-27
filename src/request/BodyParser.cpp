#include "BodyParser.hpp"

BodyParser::BodyParser() {}

BodyParser::~BodyParser() {}

std::string BodyParser::parse(const std::string& buffer, bool is_chunked,
                              size_t content_length) {
  std::string body;

  if (content_length == 0) {
    return "";
  }
  if (is_chunked) {
    body = "";
  } else {
    body = parseBody(buffer, content_length);
  }
  return body;
}

std::string BodyParser::parseBody(const std::string& buffer,
                                  size_t content_length) {
  size_t body_len = buffer.find(CRLF);
  if (body_len == std::string::npos) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "No body_len");
  }
  if (body_len > content_length) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST,
                             "body length is less than content_length");
  }
  return buffer.substr(0, body_len);
}
