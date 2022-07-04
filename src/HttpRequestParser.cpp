#include "HttpRequestParser.hpp"

HttpRequestParser::HttpRequestParser() {}

HttpRequestParser::~HttpRequestParser() {}

HttpRequest* HttpRequestParser::parse(const char* buffer) {
  current_request_ = new HttpRequest;
  current_buffer_ = buffer;
  buffer_offset_ = buffer;

  parseRequestLine();
  // parseHeaderField();
  return current_request_;
}

void HttpRequestParser::parseRequestLine() {
  std::string request_line;

  if (!getLine(request_line)) {
    return;
  }

  std::string::size_type method_end = request_line.find_first_of(" ");
  std::string::size_type uri_begin =
      request_line.find_first_not_of(" ", method_end);
  std::string::size_type uri_end = request_line.find_last_of(" ");

  std::string method = request_line.substr(0, method_end);
  std::string uri = request_line.substr(uri_begin, uri_end - uri_begin);
  std::string version = request_line.substr(uri_end + 1);

  // validateする
  // validateRequestLine();

  current_request_->setMethod(method);
  current_request_->setUri(uri);
  current_request_->setVersion(version);
  current_request_->setIsValid(true);
}

// void HttpRequestParser::parseHeaderField() {
//   std::string header_field_line;

//   while (getLine(header_field_line) && header_field_line.size() != 0) {
//     ;
//     ;
//   }
//   std::cerr << "end: " << __FUNCTION__ << std::endl;
// }

// 現在のオフセットから一行読み取る関数。読み取ったら改行の次の文字にoffsetを進める
bool HttpRequestParser::getLine(std::string& line) {
  std::string::size_type n = buffer_offset_.find("\r\n");

  if (n == std::string::npos) {
    return false;
  }
  line = buffer_offset_.substr(0, n);
  buffer_offset_ = buffer_offset_.substr(n + 2);
  return true;
}
// void HttpRequestParser::parseHeaderField();
