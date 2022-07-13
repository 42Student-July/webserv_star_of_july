#include "HttpRequestParser.hpp"

HttpRequestParser::HttpRequestParser() {}

HttpRequestParser::~HttpRequestParser() {}

const HttpRequestParser::HeaderFieldMap& HttpRequestParser::getHeaderFieldMap()
    const {
  return name_value_map_;
}

HttpRequestDTO* HttpRequestParser::parse(const char* buffer) {
  current_request_ = new HttpRequestDTO;
  current_request_->is_bad_request = false;
  current_request_->is_chunked = false;

  current_buffer_ = buffer;
  buffer_offset_ = buffer;

  parseRequestLine();
  parseHeaderField();
  return current_request_;
}

void HttpRequestParser::parseRequestLine() {
  std::string line;

  if (!getLine(&line)) {
    return;
  }

  std::string::size_type method_end = line.find_first_of(" ");
  std::string::size_type uri_begin = line.find_first_not_of(" ", method_end);
  std::string::size_type uri_end = line.find_last_of(" ");

  std::string method = line.substr(0, method_end);
  std::string uri = line.substr(uri_begin, uri_end - uri_begin);
  std::string version = line.substr(uri_end + 1);

  // validateする
  // validateRequestLine();

  current_request_->method = method;
  current_request_->uri = uri;
  current_request_->version = version;
}

void HttpRequestParser::parseHeaderField() {
  std::string line;

  while (getLine(&line) && line.size() != 0) {
    std::string::size_type name_end = line.find_first_of(":");
    std::string name = line.substr(0, name_end);
    std::string::size_type value_begin =
        line.find_first_not_of(WS, name_end + 1);
    std::string value = line.substr(value_begin);

    name_value_map_[name] = value;
  }
  // for (HeaderFieldMap::iterator it = name_value_map_.begin();
  //      it != name_value_map_.end(); ++it) {
  //   std::cerr << it->first << " = " << it->second << std::endl;
  // }
  // current_request_->host = name_value_map_["Host"];
  // current_request_->connection = name_value_map_["Connection"];
}

// 現在のオフセットから一行読み取る関数。読み取ったら改行の次の文字にoffsetを進める
bool HttpRequestParser::getLine(std::string* line) {
  std::string::size_type n = buffer_offset_.find(CRLF);

  if (n == std::string::npos) {
    return false;
  }
  *line = buffer_offset_.substr(0, n);
  buffer_offset_ = buffer_offset_.substr(n + 2);
  return true;
}

const std::string HttpRequestParser::CRLF = "\r\n";
const std::string HttpRequestParser::WS = " \t";
