#include "BodyParser.hpp"

BodyParser::BodyParser() {}

BodyParser::~BodyParser() {}

std::string BodyParser::parse(const std::string& buffer, bool is_chunked,
                              size_t content_length) {
  if (!is_chunked && content_length == 0) {
    return "";
  }
  if (is_chunked) {
    return parseChunkedBody(buffer);
  } else {
    return parseBody(buffer, content_length);
  }
}

std::string BodyParser::parseBody(const std::string& buffer,
                                  size_t content_length) {
  size_t body_len = buffer.find(CRLF);
  if (body_len == std::string::npos) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "No body_len");
  }
  if (body_len < content_length) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST,
                             "body length is less than content_length");
  }
  return buffer.substr(0, body_len);
}

std::string BodyParser::parseChunkedBody(const std::string& buffer) {
  std::string body;
  StringPos offset = 0;

  while (42) {
    std::string chunk_size_str = getLine(buffer, &offset);
    int chunk_size_num = utility::hexStringToInt(chunk_size_str);
    if (chunk_size_num == 0) {
      break;
    }
    std::string ex_chunk_data = getLine(buffer, &offset);
    if (ex_chunk_data.size() != static_cast<size_t>(chunk_size_num)) {
      throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "chunk_size error");
    }
    body += ex_chunk_data;
  }
  return body;
}

std::string BodyParser::getLine(const std::string& buffer, StringPos* offset) {
  StringPos crlf_pos = buffer.find(CRLF, *offset);
  if (crlf_pos == std::string::npos) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST, "getLine() error");
  }
  std::string line = buffer.substr(*offset, crlf_pos - *offset);
  *offset = crlf_pos + 2;
  return line;
}
