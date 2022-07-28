#include "MessageBodyParser.hpp"

MessageBodyParser::MessageBodyParser() {}

MessageBodyParser::~MessageBodyParser() {}

std::string MessageBodyParser::parse(const std::string& buffer, bool is_chunked,
                                     size_t content_length) {
  // std::cerr << "buffer:" << buffer << std::endl
  //           << "is_chunked:" << is_chunked << std::endl
  //           << "exists_content_length:" << exists_content_length << std::endl
  //           << "content_length:" << content_length << std::endl
  //           << std::endl;
  if (is_chunked) {
    return parseChunkedBody(buffer);
  } else {
    return parseBody(buffer, content_length);
  }
}

std::string MessageBodyParser::parseBody(const std::string& buffer,
                                         size_t content_length) {
  // std::cerr << buffer.size() << ", conte: " << content_length << std::endl;
  if (buffer.size() < content_length) {
    throw ParseErrorExeption(HttpStatus::BAD_REQUEST,
                             "body length is less than content_length");
  }
  return buffer.substr(0, content_length);
}

std::string MessageBodyParser::parseChunkedBody(const std::string& buffer) {
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
