#include "MessageBodyParser.hpp"

MessageBodyParser::MessageBodyParser() {}

MessageBodyParser::~MessageBodyParser() {}

// std::cerr << "buffer:" << buffer << std::endl
//           << "content_length:" << content_length << std::endl
//           << std::endl;

std::string MessageBodyParser::parseBody(const std::string& buffer,
                                         size_t content_length) {
  if (content_length > kMaxBodyLength) {
    throw ParseErrorExeption(HttpStatus::PAYLOAD_TOO_LARGE, "body is too long");
  }
  if (buffer.size() < content_length) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST,
                             "body length is less than content_length");
  }
  return buffer.substr(0, content_length);
}

std::string MessageBodyParser::parseChunkedBody(const std::string& buffer) {
  std::string body;
  StringPos offset = 0;

  if (buffer.size() > kMaxBodyLength) {
    throw ParseErrorExeption(HttpStatus::PAYLOAD_TOO_LARGE, "body is too long");
  }

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
